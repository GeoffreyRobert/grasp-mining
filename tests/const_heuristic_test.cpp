#include "gtest/gtest.h"

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/candidate_generator.h"
#include "const-heuristic/restricted_selector.h"

namespace {

TEST(ConstHeuristicTest, Operator_Should_Schedule_Single_Operation_Deterministically)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  const int makespan = 1;
  const OperationId critical_op = 1;

  const unsigned seed = 0;
  Solution solution(problem);
  CandidateGenerator generator(problem);
  RestrictedSelector selector(problem, seed);
  ConstHeuristic binato(problem, generator, selector);

  binato(solution);

  EXPECT_EQ(makespan, solution.Makespan());
  EXPECT_EQ(critical_op, solution.CriticalOp());
}

TEST(ConstHeuristicTest, Operator_Should_Be_Reusable)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  const int makespan = 1;
  const OperationId critical_op = 1;

  const unsigned seed = 0;
  Solution solution(problem);
  CandidateGenerator generator(problem);
  RestrictedSelector selector(problem, seed);
  ConstHeuristic binato(problem, generator, selector);

  Solution first_solution(problem);
  Solution second_solution(problem);

  binato(first_solution);
  binato(second_solution);

  EXPECT_EQ(makespan, second_solution.Makespan());
  EXPECT_EQ(critical_op, second_solution.CriticalOp());
}

} // namespace
