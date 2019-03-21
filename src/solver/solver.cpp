// Copyright 2010-2018 Google LLC
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "solver.h"

#include <cmath>
#include <stack>
#include <vector>

#include "ortools/base/commandlineflags.h"
#include "ortools/base/integral_types.h"
#include "ortools/base/timer.h"

#include "absl/memory/memory.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "ortools/glop/preprocessor.h"
#include "ortools/glop/status.h"
#include "ortools/lp_data/lp_types.h"
#include "ortools/lp_data/lp_utils.h"
#include "ortools/lp_data/proto_utils.h"
#include "ortools/util/fp_utils.h"

// --------------------------------------------------------
// LPSolver
// --------------------------------------------------------

Solver::Solver() : num_solves_(0) {}

void Solver::SetParameters(const Parameters& parameters) {
  parameters_ = parameters;
}

const Parameters& Solver::GetParameters() const { return parameters_; }

ProblemStatus Solver::Solve(const Problem& problem) {
  std::unique_ptr<TimeLimit> time_limit =
      TimeLimit::FromParameters(parameters_);
  return SolveWithTimeLimit(lp, time_limit.get());
}

ProblemStatus LPSolver::SolveWithTimeLimit(const Problem& problem,
                                           TimeLimit* time_limit) {
  if (time_limit == nullptr) {
    LOG(DFATAL) << "SolveWithTimeLimit() called with a nullptr time_limit.";
    return ProblemStatus::ABNORMAL;
  }
  ++num_solves_;
  num_hybrid_sequence_iterations_ = 0;
  DumpLinearProgramIfRequiredByFlags(problem, num_solves_);
  // Check some preconditions.
  if (!problem.IsCleanedUp()) {
    LOG(DFATAL) << "The columns of the given linear program should be ordered "
                << "by row and contain no zero coefficients. Call CleanUp() "
                << "on it before calling Solve().";
    ResizeSolution(problem.num_constraints(), problem.num_variables());
    return ProblemStatus::INVALID_PROBLEM;
  }
  if (!problem.IsValid()) {
    LOG(DFATAL) << "The given linear program is invalid. It contains NaNs, "
                << "infinite coefficients or invalid bounds specification. "
                << "You can construct it in debug mode to get the exact cause.";
    ResizeSolution(problem.num_constraints(), problem.num_variables());
    return ProblemStatus::INVALID_PROBLEM;
  }
  // Display a warning if assertions are enabled.
  DLOG(WARNING)
      << "\n******************************************************************"
         "\n* WARNING: Glop will be very slow because it will use DCHECKs    *"
         "\n* to verify the results and the precision of the solver.         *"
         "\n* You can gain at least an order of magnitude speedup by         *"
         "\n* compiling with optimizations enabled and by defining NDEBUG.   *"
         "\n******************************************************************";

  // Note that we only activate the floating-point exceptions after we are sure
  // that the program is valid. This way, if we have input NaNs, we will not
  // crash.
  ScopedFloatingPointEnv scoped_fenv;
  if (FLAGS_lp_solver_enable_fp_exceptions) {
#ifdef _MSC_VER
    scoped_fenv.EnableExceptions(_EM_INVALID | EM_ZERODIVIDE);
#else
    scoped_fenv.EnableExceptions(FE_DIVBYZERO | FE_INVALID);
#endif
  }

  // Make an internal copy of the problem for the preprocessing.
  VLOG(1) << "Initial problem: " << problem.GetDimensionString();
  VLOG(1) << "Objective stats: " << problem.GetObjectiveStatsString();
  current_problem_.PopulateFromProblem(problem);

  // Preprocess.
  MainProblemPreprocessor preprocessor(&parameters_);
  preprocessor.SetTimeLimit(time_limit);

  const bool postsolve_is_needed = preprocessor.Run(&current_problem_);

  VLOG(1) << "Presolved problem: "
          << current_problem_.GetDimensionString();

  // At this point, we need to initialize a ProblemSolution with the correct
  // size and status.
  ProblemSolution solution(current_problem_.num_constraints(),
                           current_problem_.num_variables());
  solution.status = preprocessor.status();

  // Do not launch the solver if the time limit was already reached. This might
  // mean that the pre-processors were not all run, and current_linear_program_
  // might not be in a completely safe state.
  if (!time_limit->LimitReached()) {
    RunRevisedSimplexIfNeeded(&solution, time_limit);
  }

  if (postsolve_is_needed) preprocessor.RecoverSolution(&solution);
  const ProblemStatus status = LoadAndVerifySolution(lp, solution);

  // LOG some statistics that can be parsed by our benchmark script.
  VLOG(1) << "status: " << status;
  VLOG(1) << "objective: " << GetObjectiveValue();
  VLOG(1) << "iterations: " << GetNumberOfSimplexIterations();
  VLOG(1) << "time: " << time_limit->GetElapsedTime();
  VLOG(1) << "deterministic_time: "
          << time_limit->GetElapsedDeterministicTime();

  return status;
}

void LPSolver::Clear() {
  ResizeSolution(RowIndex(0), ColIndex(0));
  revised_simplex_.reset(nullptr);
}

void LPSolver::SetInitialBasis(
    const VariableStatusRow& variable_statuses,
    const ConstraintStatusColumn& constraint_statuses) {
  // Create the associated basis state.
  BasisState state;
  state.statuses = variable_statuses;
  for (const ConstraintStatus status : constraint_statuses) {
    // Note the change of upper/lower bound between the status of a constraint
    // and the status of its associated slack variable.
    switch (status) {
      case ConstraintStatus::FREE:
        state.statuses.push_back(VariableStatus::FREE);
        break;
      case ConstraintStatus::AT_LOWER_BOUND:
        state.statuses.push_back(VariableStatus::AT_UPPER_BOUND);
        break;
      case ConstraintStatus::AT_UPPER_BOUND:
        state.statuses.push_back(VariableStatus::AT_LOWER_BOUND);
        break;
      case ConstraintStatus::FIXED_VALUE:
        state.statuses.push_back(VariableStatus::FIXED_VALUE);
        break;
      case ConstraintStatus::BASIC:
        state.statuses.push_back(VariableStatus::BASIC);
        break;
    }
  }
  if (revised_simplex_ == nullptr) {
    revised_simplex_ = absl::make_unique<RevisedSimplex>();
  }
  revised_simplex_->LoadStateForNextSolve(state);
  if (parameters_.use_preprocessing()) {
    LOG(WARNING) << "In GLOP, SetInitialBasis() was called but the parameter "
                    "use_preprocessing is true, this will likely not result in "
                    "what you want.";
  }
}

namespace {
// Computes the "real" problem objective from the one without offset nor
// scaling.
Fractional ProblemObjectiveValue(const LinearProgram& lp, Fractional value) {
  return lp.objective_scaling_factor() * (value + lp.objective_offset());
}

// Returns the allowed error magnitude for something that should evaluate to
// value under the given tolerance.
Fractional AllowedError(Fractional tolerance, Fractional value) {
  return tolerance * std::max(1.0, std::abs(value));
}
}  // namespace

// TODO(user): Try to also check the precision of an INFEASIBLE or UNBOUNDED
// return status.
ProblemStatus LPSolver::LoadAndVerifySolution(const LinearProgram& lp,
                                              const ProblemSolution& solution) {
  if (!IsProblemSolutionConsistent(lp, solution)) {
    VLOG(1) << "Inconsistency detected in the solution.";
    ResizeSolution(lp.num_constraints(), lp.num_variables());
    return ProblemStatus::ABNORMAL;
  }

  // Load the solution.
  primal_values_ = solution.primal_values;
  dual_values_ = solution.dual_values;
  variable_statuses_ = solution.variable_statuses;
  constraint_statuses_ = solution.constraint_statuses;
  ProblemStatus status = solution.status;

  // Objective before eventually moving the primal/dual values inside their
  // bounds.
  ComputeReducedCosts(lp);
  const Fractional primal_objective_value = ComputeObjective(lp);
  const Fractional dual_objective_value = ComputeDualObjective(lp);
  VLOG(1) << "Primal objective (before moving primal/dual values) = "
          << absl::StrFormat("%.15E",
                             ProblemObjectiveValue(lp, primal_objective_value));
  VLOG(1) << "Dual objective (before moving primal/dual values) = "
          << absl::StrFormat("%.15E",
                             ProblemObjectiveValue(lp, dual_objective_value));

  // Eventually move the primal/dual values inside their bounds.
  if (status == ProblemStatus::OPTIMAL &&
      parameters_.provide_strong_optimal_guarantee()) {
    MovePrimalValuesWithinBounds(lp);
    MoveDualValuesWithinBounds(lp);
  }

  // The reported objective to the user.
  problem_objective_value_ = ProblemObjectiveValue(lp, ComputeObjective(lp));
  VLOG(1) << "Primal objective (after moving primal/dual values) = "
          << absl::StrFormat("%.15E", problem_objective_value_);

  ComputeReducedCosts(lp);
  ComputeConstraintActivities(lp);

  // These will be set to true if the associated "infeasibility" is too large.
  //
  // The tolerance used is the parameter solution_feasibility_tolerance. To be
  // somewhat independent of the original problem scaling, the thresholds used
  // depend of the quantity involved and of its coordinates:
  // - tolerance * max(1.0, abs(cost[col])) when a reduced cost is infeasible.
  // - tolerance * max(1.0, abs(bound)) when a bound is crossed.
  // - tolerance for an infeasible dual value (because the limit is always 0.0).
  bool rhs_perturbation_is_too_large = false;
  bool cost_perturbation_is_too_large = false;
  bool primal_infeasibility_is_too_large = false;
  bool dual_infeasibility_is_too_large = false;
  bool primal_residual_is_too_large = false;
  bool dual_residual_is_too_large = false;

  // Computes all the infeasiblities and update the Booleans above.
  ComputeMaxRhsPerturbationToEnforceOptimality(lp,
                                               &rhs_perturbation_is_too_large);
  ComputeMaxCostPerturbationToEnforceOptimality(
      lp, &cost_perturbation_is_too_large);
  const double primal_infeasibility =
      ComputePrimalValueInfeasibility(lp, &primal_infeasibility_is_too_large);
  const double dual_infeasibility =
      ComputeDualValueInfeasibility(lp, &dual_infeasibility_is_too_large);
  const double primal_residual =
      ComputeActivityInfeasibility(lp, &primal_residual_is_too_large);
  const double dual_residual =
      ComputeReducedCostInfeasibility(lp, &dual_residual_is_too_large);

  // TODO(user): the name is not really consistent since in practice those are
  // the "residual" since the primal/dual infeasibility are zero when
  // parameters_.provide_strong_optimal_guarantee() is true.
  max_absolute_primal_infeasibility_ =
      std::max(primal_infeasibility, primal_residual);
  max_absolute_dual_infeasibility_ =
      std::max(dual_infeasibility, dual_residual);
  VLOG(1) << "Max. primal infeasibility = "
          << max_absolute_primal_infeasibility_;
  VLOG(1) << "Max. dual infeasibility = " << max_absolute_dual_infeasibility_;

  // Now that all the relevant quantities are computed, we check the precision
  // and optimality of the result. See Chvatal pp. 61-62. If any of the tests
  // fail, we return the IMPRECISE status.
  const double objective_error_ub = ComputeMaxExpectedObjectiveError(lp);
  VLOG(1) << "Objective error <= " << objective_error_ub;

  if (status == ProblemStatus::OPTIMAL &&
      parameters_.provide_strong_optimal_guarantee()) {
    // If the primal/dual values were moved to the bounds, then the primal/dual
    // infeasibilities should be exactly zero (but not the residuals).
    if (primal_infeasibility != 0.0 || dual_infeasibility != 0.0) {
      LOG(ERROR) << "Primal/dual values have been moved to their bounds. "
                 << "Therefore the primal/dual infeasibilities should be "
                 << "exactly zero (but not the residuals). If this message "
                 << "appears, there is probably a bug in "
                 << "MovePrimalValuesWithinBounds() or in "
                 << "MoveDualValuesWithinBounds().";
    }
    if (rhs_perturbation_is_too_large) {
      VLOG(1) << "The needed rhs perturbation is too large !!";
      status = ProblemStatus::IMPRECISE;
    }
    if (cost_perturbation_is_too_large) {
      VLOG(1) << "The needed cost perturbation is too large !!";
      status = ProblemStatus::IMPRECISE;
    }
  }

  // Note that we compare the values without offset nor scaling. We also need to
  // compare them before we move the primal/dual values, otherwise we lose some
  // precision since the values are modified independently of each other.
  if (status == ProblemStatus::OPTIMAL) {
    if (std::abs(primal_objective_value - dual_objective_value) >
        objective_error_ub) {
      VLOG(1) << "The objective gap of the final solution is too large.";
      status = ProblemStatus::IMPRECISE;
    }
  }
  if ((status == ProblemStatus::OPTIMAL ||
       status == ProblemStatus::PRIMAL_FEASIBLE) &&
      (primal_residual_is_too_large || primal_infeasibility_is_too_large)) {
    VLOG(1) << "The primal infeasibility of the final solution is too large.";
    status = ProblemStatus::IMPRECISE;
  }
  if ((status == ProblemStatus::OPTIMAL ||
       status == ProblemStatus::DUAL_FEASIBLE) &&
      (dual_residual_is_too_large || dual_infeasibility_is_too_large)) {
    VLOG(1) << "The dual infeasibility of the final solution is too large.";
    status = ProblemStatus::IMPRECISE;
  }

  may_have_multiple_solutions_ =
      (status == ProblemStatus::OPTIMAL) ? IsOptimalSolutionOnFacet(lp) : false;
  return status;
}

Fractional LPSolver::GetObjectiveValue() const {
  return problem_objective_value_;
}

double LPSolver::DeterministicTime() const {
  return revised_simplex_ == nullptr ? 0.0
                                     : revised_simplex_->DeterministicTime();
}

namespace {

void LogVariableStatusError(ColIndex col, Fractional value,
                            VariableStatus status, Fractional lb,
                            Fractional ub) {
  VLOG(1) << "Variable " << col << " status is "
          << GetVariableStatusString(status) << " but its value is " << value
          << " and its bounds are [" << lb << ", " << ub << "].";
}

void LogConstraintStatusError(RowIndex row, ConstraintStatus status,
                              Fractional lb, Fractional ub) {
  VLOG(1) << "Constraint " << row << " status is "
          << GetConstraintStatusString(status) << " but its bounds are [" << lb
          << ", " << ub << "].";
}

}  // namespace

bool LPSolver::IsProblemSolutionConsistent(
    const LinearProgram& lp, const ProblemSolution& solution) const {
  const RowIndex num_rows = lp.num_constraints();
  const ColIndex num_cols = lp.num_variables();
  if (solution.variable_statuses.size() != num_cols) return false;
  if (solution.constraint_statuses.size() != num_rows) return false;
  if (solution.primal_values.size() != num_cols) return false;
  if (solution.dual_values.size() != num_rows) return false;
  if (solution.status != ProblemStatus::OPTIMAL &&
      solution.status != ProblemStatus::PRIMAL_FEASIBLE &&
      solution.status != ProblemStatus::DUAL_FEASIBLE) {
    return true;
  }

  // This checks that the variable statuses verify the properties described
  // in the VariableStatus declaration.
  RowIndex num_basic_variables(0);
  for (ColIndex col(0); col < num_cols; ++col) {
    const Fractional value = solution.primal_values[col];
    const Fractional lb = lp.variable_lower_bounds()[col];
    const Fractional ub = lp.variable_upper_bounds()[col];
    const VariableStatus status = solution.variable_statuses[col];
    switch (solution.variable_statuses[col]) {
      case VariableStatus::BASIC:
        // TODO(user): Check that the reduced cost of this column is epsilon
        // close to zero.
        ++num_basic_variables;
        break;
      case VariableStatus::FIXED_VALUE:
        if (lb != ub || value != lb) {
          LogVariableStatusError(col, value, status, lb, ub);
          return false;
        }
        break;
      case VariableStatus::AT_LOWER_BOUND:
        if (value != lb || lb == ub) {
          LogVariableStatusError(col, value, status, lb, ub);
          return false;
        }
        break;
      case VariableStatus::AT_UPPER_BOUND:
        // TODO(user): revert to an exact comparison once the bug causing this
        // to fail has been fixed.
        if (!AreWithinAbsoluteTolerance(value, ub, 1e-7) || lb == ub) {
          LogVariableStatusError(col, value, status, lb, ub);
          return false;
        }
        break;
      case VariableStatus::FREE:
        if (lb != -kInfinity || ub != kInfinity || value != 0.0) {
          LogVariableStatusError(col, value, status, lb, ub);
          return false;
        }
        break;
    }
  }
  for (RowIndex row(0); row < num_rows; ++row) {
    const Fractional dual_value = solution.dual_values[row];
    const Fractional lb = lp.constraint_lower_bounds()[row];
    const Fractional ub = lp.constraint_upper_bounds()[row];
    const ConstraintStatus status = solution.constraint_statuses[row];

    // The activity value is not checked since it is imprecise.
    // TODO(user): Check that the activity is epsilon close to the expected
    // value.
    switch (status) {
      case ConstraintStatus::BASIC:
        if (dual_value != 0.0) {
          VLOG(1) << "Constraint " << row << " is BASIC, but its dual value is "
                  << dual_value << " instead of 0.";
          return false;
        }
        ++num_basic_variables;
        break;
      case ConstraintStatus::FIXED_VALUE:
        if (lb != ub) {
          LogConstraintStatusError(row, status, lb, ub);
          return false;
        }
        break;
      case ConstraintStatus::AT_LOWER_BOUND:
        if (lb == -kInfinity) {
          LogConstraintStatusError(row, status, lb, ub);
          return false;
        }
        break;
      case ConstraintStatus::AT_UPPER_BOUND:
        if (ub == kInfinity) {
          LogConstraintStatusError(row, status, lb, ub);
          return false;
        }
        break;
      case ConstraintStatus::FREE:
        if (dual_value != 0.0) {
          VLOG(1) << "Constraint " << row << " is FREE, but its dual value is "
                  << dual_value << " instead of 0.";
          return false;
        }
        if (lb != -kInfinity || ub != kInfinity) {
          LogConstraintStatusError(row, status, lb, ub);
          return false;
        }
        break;
    }
  }

  // TODO(user): We could check in debug mode (because it will be costly) that
  // the basis is actually factorizable.
  if (num_basic_variables != num_rows) {
    VLOG(1) << "Wrong number of basic variables: " << num_basic_variables;
    return false;
  }
  return true;
}

double LPSolver::ComputeObjective(const LinearProgram& lp) {
  const ColIndex num_cols = lp.num_variables();
  DCHECK_EQ(num_cols, primal_values_.size());
  KahanSum sum;
  for (ColIndex col(0); col < num_cols; ++col) {
    sum.Add(lp.objective_coefficients()[col] * primal_values_[col]);
  }
  return sum.Value();
}
