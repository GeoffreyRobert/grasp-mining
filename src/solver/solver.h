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

#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <memory>

#include "grasp-mining/solver/parameters.h"
#include "grasp-mining/data/problem_data.h"
#include "grasp-mining/util/time_limit.h"

class Solver {
public:
	Solver();

	// Sets and gets the solver parameters.
	void SetParameters(const Parameters& parameters);
	const Parameters& GetParameters() const;

	// Solves the given JSSP and returns the solve status. See the ProblemStatus 
	// documentation for a description of the different values.
	ProblemStatus Solve(const Problem& problem);

	// Same as Solve() but use the given time limit.
	ProblemStatus SolveWithTimeLimit(const Problem& problem,
									 TimeLimit* time_limit);

	// Puts the solver in a clean state.
	void Clear();

	// This loads a given solution and computes related quantities so that the
	// getters below will refer to it.
	ProblemStatus LoadAndVerifySolution(const Problem& problem,
										const ProblemSolution& solution);

	// Returns the objective value of the solution with its offset and scaling.
	Fractional GetObjectiveValue() const;

	// Returns the number of evaluation iterations used by the last Solve().
	int GetNumberOfEvaluationIterations() const;

	// Returns the "deterministic time" since the creation of the solver. Note
	// That this time is only increased when some operations take place in this
	// class.
	double DeterministicTime() const;

 private:
	// Checks that the returned solution values and statuses are consistent.
	// Returns true if this is the case. See the code for the exact check
	// performed.
	bool IsProblemSolutionConsistent(const Problem& problem,
									 const ProblemSolution& solution) const;

	// Computes derived quantities from the solution.
	void ComputeMakespan(const Problem& problem);
	void ComputeMachineWorkload(const Problem& problem);

	// Computes the primal/dual objectives (without the offset). Note that the
	// dual objective needs the reduced costs in addition to the dual values.
	double ComputeObjective(const Problem& problem);

	// On a call to Solve(), this is initialized to an exact copy of the given
	// linear program. It is later modified by the preprocessors and then solved
	// by the revised simplex.
	//
	// This is not efficient memory-wise but allows to check optimality with
	// respect to the given LinearProgram that is guaranteed to not have been
	// modified. It also allows for a nicer Solve() API with a const
	// LinearProgram& input.
	Problem current_problem_;

	// The hybrid solver.
	std::unique_ptr<Generator> generator_;
	std::unique_ptr<DataMiner> data_miner_;
	std::unique_ptr<Controler> controler_;

	// The number of full sequence iterations used by the last Solve().
	int num_hybrid_sequence_iterations_;

	// Object holding all the parameters of the algorithm.
	Parameters parameters_;
};

#endif  // _SOLVER_H_
