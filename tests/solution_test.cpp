#include "gtest/gtest.h"

#include "data/solution.h"

namespace {

TEST(SolutionTest, GetOperationScheduling_Should_Initialize_Operation)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  Solution solution(problem);

  const OperationId oid = 1;
  const int start_date = 0;
  const int end_date = 1;
  const MachineId parent_on_mac = problem.OriginOp;
  const MachineId child_on_mac = problem.FinalOp;

  solution.GetOperationScheduling(oid);

  EXPECT_EQ(start_date, solution.startDate[oid]);
  EXPECT_EQ(end_date, solution.endDate[oid]);
  EXPECT_EQ(parent_on_mac, solution.macParent[oid]);
  EXPECT_EQ(child_on_mac, solution.macChild[oid]);
}

TEST(SolutionTest, AddOperation_Should_Update_Child_On_Machine_Of_Parent_And_Makespan)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  Solution solution(problem);

  const OperationId tested_oid = 1;
  const OperationId parent_oid = problem.OriginOp;
  const OperationId child_oid = problem.FinalOp;
  const MachineId parent_child_on_mac = tested_oid;
  const MachineId child_parent_on_mac = tested_oid;
  const int makespan = 1;
  const OperationId critical_op = tested_oid;

  solution.GetOperationScheduling(tested_oid);
  solution.AddOperation(tested_oid);

  EXPECT_EQ(parent_child_on_mac, solution.macChild[parent_oid]);
  EXPECT_EQ(child_parent_on_mac, solution.macParent[child_oid]);
  EXPECT_EQ(makespan, solution.makespan);
  EXPECT_EQ(critical_op, solution.criticalOp);
}

TEST(SolutionTest, GetOperationScheduling_Should_Initialize_Same_Job_Operations)
{
  Problem problem(1, 2, 2,
    vector<std::pair<MachineId, int>> {
      { 0, 1 }, { 1, 1 },
    });
  Solution solution(problem);

  const OperationId tested_oid = 2;
  const OperationId parent_oid = 1;
  const int start_date = 1;
  const int end_date = 2;
  const MachineId parent_on_mac = problem.OriginOp;
  const MachineId child_on_mac = problem.FinalOp;

  solution.GetOperationScheduling(parent_oid);
  solution.AddOperation(parent_oid);
  solution.GetOperationScheduling(tested_oid);

  EXPECT_EQ(start_date, solution.startDate[tested_oid]);
  EXPECT_EQ(end_date, solution.endDate[tested_oid]);
  EXPECT_EQ(parent_on_mac, solution.macParent[tested_oid]);
  EXPECT_EQ(child_on_mac, solution.macChild[tested_oid]);
}

TEST(SolutionTest, AddOperation_Should_Update_Children_On_Same_Machine)
{
  Problem problem(3, 1, 1,
    vector<std::pair<MachineId, int>> {
      { 0, 1 },
      { 0, 1 },
      { 0, 1 },
    });
  Solution solution(problem);

  const OperationId tested_oid = 1;
  const OperationId parent_oid = problem.OriginOp;
  const OperationId child2_oid = 2;
  const OperationId child3_oid = 3;
  const MachineId child2_parent_on_mac = tested_oid;
  const MachineId child3_parent_on_mac = tested_oid;
  const int makespan = 1;
  const OperationId critical_op = tested_oid;

  solution.GetOperationScheduling(tested_oid);
  solution.AddOperation(tested_oid);

  EXPECT_EQ(parent_oid, solution.macParent[tested_oid]);
  EXPECT_EQ(child2_parent_on_mac, solution.macParent[child2_oid]);
  EXPECT_EQ(child3_parent_on_mac, solution.macParent[child3_oid]);
  EXPECT_EQ(makespan, solution.makespan);
  EXPECT_EQ(critical_op, solution.criticalOp);
}

TEST(SolutionTest, GetOperationScheduling_Should_Initialize_Same_Machine_Operations)
{
  Problem problem(2, 1, 2,
    vector<std::pair<MachineId, int>> {
      { 0, 1 },
      { 0, 1 },
    });
  Solution solution(problem);

  const OperationId parent_oid = 1;
  const OperationId tested_oid = 2;
  const int start_date = 1;
  const int end_date = 2;
  const MachineId parent_on_mac = parent_oid;
  const MachineId child_on_mac = problem.FinalOp;
  const MachineId parent_child_on_mac = problem.FinalOp; // tested_oid is not "added" in this test, only scheduled
  const int makespan = 1;
  const OperationId critical_op = parent_oid; // for the same reasons, makespan is not updated

  solution.GetOperationScheduling(parent_oid);
  solution.AddOperation(parent_oid);
  solution.GetOperationScheduling(tested_oid);

  EXPECT_EQ(start_date, solution.startDate[tested_oid]);
  EXPECT_EQ(end_date, solution.endDate[tested_oid]);
  EXPECT_EQ(parent_on_mac, solution.macParent[tested_oid]);
  EXPECT_EQ(child_on_mac, solution.macChild[tested_oid]);
  EXPECT_EQ(parent_child_on_mac, solution.macChild[parent_oid]);
  EXPECT_EQ(makespan, solution.makespan);
  EXPECT_EQ(critical_op, solution.criticalOp);
}

} // namespace
