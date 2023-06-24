#include <limits>
#include "gtest/gtest.h"

#include "data/problem.h"
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

  solution.ScheduleOperation(oid);

  EXPECT_EQ(start_date, solution.StartDate(oid));
  EXPECT_EQ(end_date, solution.EndDate(oid));
  EXPECT_EQ(parent_on_mac, solution.ParentOnMachine(oid));
  EXPECT_EQ(child_on_mac, solution.ChildOnMachine(oid));
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

  solution.ScheduleOperation(tested_oid);
  solution.AddOperation(tested_oid);

  EXPECT_EQ(parent_child_on_mac, solution.ChildOnMachine(parent_oid));
  EXPECT_EQ(child_parent_on_mac, solution.ParentOnMachine(child_oid));
  EXPECT_EQ(makespan, solution.Makespan());
  EXPECT_EQ(critical_op, solution.CriticalOp());
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

  solution.ScheduleOperation(parent_oid);
  solution.AddOperation(parent_oid);
  solution.ScheduleOperation(tested_oid);

  EXPECT_EQ(start_date, solution.StartDate(tested_oid));
  EXPECT_EQ(end_date, solution.EndDate(tested_oid));
  EXPECT_EQ(parent_on_mac, solution.ParentOnMachine(tested_oid));
  EXPECT_EQ(child_on_mac, solution.ChildOnMachine(tested_oid));
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

  solution.ScheduleOperation(tested_oid);
  solution.AddOperation(tested_oid);

  EXPECT_EQ(parent_oid, solution.ParentOnMachine(tested_oid));
  EXPECT_EQ(child2_parent_on_mac, solution.ParentOnMachine(child2_oid));
  EXPECT_EQ(child3_parent_on_mac, solution.ParentOnMachine(child3_oid));
  EXPECT_EQ(makespan, solution.Makespan());
  EXPECT_EQ(critical_op, solution.CriticalOp());
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

  solution.ScheduleOperation(parent_oid);
  solution.AddOperation(parent_oid);
  solution.ScheduleOperation(tested_oid);

  EXPECT_EQ(start_date, solution.StartDate(tested_oid));
  EXPECT_EQ(end_date, solution.EndDate(tested_oid));
  EXPECT_EQ(parent_on_mac, solution.ParentOnMachine(tested_oid));
  EXPECT_EQ(child_on_mac, solution.ChildOnMachine(tested_oid));
  EXPECT_EQ(parent_child_on_mac, solution.ChildOnMachine(parent_oid));
  EXPECT_EQ(makespan, solution.Makespan());
  EXPECT_EQ(critical_op, solution.CriticalOp());
}

TEST(SolutionTest, AddOperation_Should_Throw_If_Same_Operation_Added_Twice)
{
  Problem problem(3, 1, 3,
    vector<std::pair<MachineId, int>> {
      { 0, 1 },
      { 0, 1 },
      { 0, 1 },
    });
  Solution solution(problem);

  const OperationId tested_oid = 1;
  const OperationId second_oid = 2;
  const OperationId third_oid = 3;

  solution.ScheduleOperation(tested_oid);
  solution.AddOperation(tested_oid);
  solution.ScheduleOperation(second_oid);
  solution.AddOperation(second_oid);
  solution.ScheduleOperation(third_oid);
  solution.AddOperation(third_oid);

  EXPECT_EQ(problem.OriginOp, solution.ParentOnMachine(tested_oid));
  EXPECT_EQ(tested_oid, solution.ParentOnMachine(second_oid));
  EXPECT_EQ(second_oid, solution.ParentOnMachine(third_oid));
  EXPECT_EQ(second_oid, solution.ChildOnMachine(tested_oid));
  EXPECT_EQ(third_oid, solution.ChildOnMachine(second_oid));
  EXPECT_EQ(problem.FinalOp, solution.ChildOnMachine(third_oid));

  // Should not throw since last on machine -> no problem
  solution.AddOperation(third_oid);

  EXPECT_THROW(solution.AddOperation(tested_oid), InvalidScheduling);
}

TEST(SolutionTest, SwapOperations_Should_Reassign_CriticalOp_On_Swap_Only_If_Child_Is_CriticalOp)
{
  Problem problem(2, 2, 3,
    vector<std::pair<MachineId, int>> {
      { 0, 1 }, { 1, 1 },
      { 0, 1 }, { 1, 1 },
    });
  Solution solution(problem);

  solution.ScheduleOperation(1);
  solution.AddOperation(1);
  solution.ScheduleOperation(3);
  solution.AddOperation(3);
  solution.ScheduleOperation(4);
  solution.AddOperation(4);
  solution.ScheduleOperation(2);
  solution.AddOperation(2);

  EXPECT_EQ(2, solution.CriticalOp());
  EXPECT_EQ(4, solution.Makespan());

  solution.SwapOperations(1, 3);

  EXPECT_EQ(2, solution.CriticalOp());
  EXPECT_EQ(4, solution.Makespan());

  solution.SwapOperations(4, 2);

  EXPECT_EQ(4, solution.CriticalOp());
  EXPECT_EQ(4, solution.Makespan());
}

TEST(SolutionTest, RescheduleOperation_Should_Update_Makespan)
{
  Problem problem(1, 1, 1,
    vector<std::pair<MachineId, int>> { { 0, 1 } });
  Solution solution(problem);

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;
  const int Mn = std::numeric_limits<int>::min();
  const int Mx = std::numeric_limits<int>::max();

  vector<int> startDate = {
   Mn,
    1,
    2,
  };
  vector<int> endDate = {
    0,
    2,
   Mx,
  };
  vector<OperationId> macParent = {
    Or,
    Or,
     1,
  };
  vector<OperationId> macChild = {
     1,
    Fn,
    Fn,
  };
  vector<bool> isCritMachine = {
    0,
    0,
    1,
  };
  solution.Initialize(
      std::move(startDate)
    , std::move(endDate)
    , std::move(macParent)
    , std::move(macChild)
    , std::move(isCritMachine)
  );

  EXPECT_EQ(1, solution.CriticalOp());
  EXPECT_EQ(2, solution.Makespan());

  int end_date = solution.RescheduleOperation(1);

  EXPECT_EQ(1, solution.CriticalOp());
  EXPECT_EQ(1, solution.Makespan());
  EXPECT_EQ(end_date, solution.Makespan());
}

} // namespace
