#include "gtest/gtest.h"

#include "const-heuristic/binato_heuristic.h"

namespace {

TEST(BinatoHeuristic, Operator_Should_Schedule_Single_Operation_Deterministically)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  const int makespan = 1;
  const OperationId critical_op = 1;

  Solution solution(problem);
  BinatoHeuristic binato(problem);

  binato(solution);

  EXPECT_EQ(makespan, solution.makespan);
  EXPECT_EQ(critical_op, solution.criticalOp);
}

TEST(BinatoHeuristic, Operator_Should_Be_Reusable)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  const int makespan = 1;
  const OperationId critical_op = 1;

  Solution first_solution(problem);
  Solution second_solution(problem);
  BinatoHeuristic binato(problem);

  binato(first_solution);
  binato(second_solution);

  EXPECT_EQ(makespan, second_solution.makespan);
  EXPECT_EQ(critical_op, second_solution.criticalOp);
}

} // namespace
