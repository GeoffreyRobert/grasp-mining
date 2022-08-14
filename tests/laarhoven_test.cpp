#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "data/problem.h"
#include "data/solution.h"
#include "local-search/laarhoven_search.h"

using std::string;

namespace {

TEST(LaarhovenTest, SimpleImprovementWithOneSwap)
{
  /*
   * 0  01
   * 1 0  1
   */
  Problem problem(2, 2, 55,
    std::vector<std::pair<MachineId, int>> {
      { 1, 1 }, { 0, 1 },
      { 0, 1 }, { 1, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
    0,
    0, 1,
    2, 3,
    4,
  };
  solution.endDate = {
    0,
    1, 2,
    3, 4,
    4,
  };
  solution.macParent = {
    Or,
    Or, Or,
     2,  1,
     4,
  };
  solution.macChild = {
     1,
     4,  3,
    Fn, Fn,
    Fn,
  };
  solution.isCritMachine = {
    0,
    0, 0,
    1, 0,
    1,
  };
  solution.makespan = 4;
  solution.criticalOp = 4;

  const int final_makespan = 2;
  const int final_criticalOp = 2;

  LaarhovenSearch local_search(problem);
  local_search(solution);

  EXPECT_EQ(solution.makespan, final_makespan);
  EXPECT_EQ(solution.criticalOp, final_criticalOp);
}

TEST(LaarhovenTest, NoImprovement)
{
  /*
   * 0 01
   * 1  01
   */
  Problem problem(2, 2, 3,
    std::vector<std::pair<MachineId, int>> {
      { 0, 1 }, { 1, 1 },
      { 0, 1 }, { 1, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
    0,
    0, 1,
    1, 2,
    3,
  };
  solution.endDate = {
    0,
    1, 2,
    2, 3,
    3,
  };
  solution.macParent = {
    Or,
    Or, Or,
     1,  2,
     4,
  };
  solution.macChild = {
     1,
     3,  4,
    Fn, Fn,
    Fn,
  };
  solution.isCritMachine = {
    0,
    0, 0,
    1, 1,
    1,
  };
  solution.makespan = 3;
  solution.criticalOp = 4;

  const int final_makespan = 3;
  const int final_criticalOp = 4;

  LaarhovenSearch local_search(problem);
  local_search(solution);

  EXPECT_EQ(solution.makespan, final_makespan);
  EXPECT_EQ(solution.criticalOp, final_criticalOp);
}

TEST(LaarhovenTest, NoImprovementShouldNotChangeSolution)
{
  /*
   * 0  01
   * 1 1 0
   * 2 0  1
   */
  Problem problem(2, 3, 4,
    std::vector<std::pair<MachineId, int>> {
      { 2, 1 }, { 0, 1 }, { 1, 1 },
      { 1, 1 }, { 0, 1 }, { 2, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
    0,
    0, 1, 2,
    0, 2, 3,
    4,
  };
  solution.endDate = {
    0,
    1, 2, 3,
    1, 3, 4,
    4,
  };
  solution.macParent = {
    Or,
    Or, Or, 4,
    Or,  2, 1,
     6,
  };
  solution.macChild = {
     1,
     6,  5, Fn,
     3, Fn, Fn,
    Fn,
  };
  solution.isCritMachine = {
    0,
    0, 0, 0,
    0, 1, 0,
    1,
  };
  solution.criticalOp = 6;
  solution.makespan = 4;

  const int final_criticalOp = 6;
  const int final_makespan = 4;

  LaarhovenSearch local_search(problem);
  local_search(solution);

  EXPECT_EQ(solution.makespan, final_makespan);
  EXPECT_EQ(solution.criticalOp, final_criticalOp);
}

TEST(LaarhovenTest, OperationNotInvolvedInSwapsShouldStillBeCompacted)
{
  /*
   * 0  012
   * 1 0
   */
  Problem problem(3, 2, 3,
    std::vector<std::pair<MachineId, int>> {
      { 0, 1 }, { 1, 1 },
      { 0, 0 }, { 1, 1 },
      { 0, 0 }, { 1, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
    0,
    0, 1,
    0, 2,
    0, 3,
    4,
  };
  solution.endDate = {
    0,
    1, 2,
    0, 3,
    0, 4,
    4,
  };
  solution.macParent = {
    Or,
     3, Or,
     5,  2,
    Or,  4,
     6,
  };
  solution.macChild = {
    1,
    Fn,  4,
     1,  6,
     3, Fn,
    Fn,
  };
  solution.isCritMachine = {
    0,
    1, 0,
    1, 1,
    0, 1,
    1,
  };
  solution.criticalOp = 6;
  solution.makespan = 4;

  const int final_criticalOp = 6;
  const int final_makespan = 3;

  LaarhovenSearch local_search(problem);
  local_search(solution);

  EXPECT_EQ(solution.makespan, final_makespan);
  EXPECT_EQ(solution.criticalOp, final_criticalOp);
}

TEST(LaarhovenTest, UnmodifiedOpsShouldBeTakenIntoAccountOnCriticalPath)
{
  /*
   * 0 1 200
   * 1 02   1
   * 2  0 21
   */
  Problem problem(3, 3, 5,
    std::vector<std::pair<MachineId, int>> {
      { 1, 1 }, { 2, 1 }, { 0, 2 },
      { 0, 1 }, { 2, 1 }, { 1, 1 },
      { 1, 1 }, { 0, 1 }, { 2, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
    0,
    0, 1, 3,
    0, 4, 5,
    1, 2, 3,
    6,
  };
  solution.endDate = {
    0,
    1, 2, 5,
    1, 5, 6,
    2, 3, 4,
    6,
  };
  solution.macParent = {
    Or,
    Or, Or, 8,
    Or,  9, 7,
     1,  4, 2,
     6,
  };
  solution.macChild = {
     1,
     7,  9, Fn,
     8, Fn, Fn,
     6,  3,  5,
    Fn,
  };
  solution.isCritMachine = {
    0,
    0, 0, 1,
    0, 1, 0,
    1, 0, 0,
    1,
  };
  solution.criticalOp = 6;
  solution.makespan = 6;

  const int final_criticalOp = 3;
  const int final_makespan = 5;

  LaarhovenSearch local_search(problem);
  local_search(solution);

  EXPECT_EQ(solution.makespan, final_makespan);
  EXPECT_EQ(solution.criticalOp, final_criticalOp);
}

TEST(LaarhovenTest, LargeProblem)
{
  Problem problem(6, 6, 55,
    std::vector<std::pair<MachineId, int>> {
      { 2, 1 }, { 0, 3 }, { 1, 6 }, { 3, 7 }, { 5, 3 }, { 4, 6 },
      { 1, 8 }, { 2, 5 }, { 4, 10 }, { 5, 10 }, { 0, 10 }, { 3, 4 },
      { 2, 5 }, { 3, 4 }, { 5, 8 }, { 0, 9 }, { 1, 1 }, { 4, 7 },
      { 1, 5 }, { 0, 5 }, { 2, 5 }, { 3, 3 }, { 4, 8 }, { 5, 9 },
      { 2, 9 }, { 1, 3 }, { 4, 5 }, { 5, 4 }, { 0, 3 }, { 3, 1 },
      { 1, 3 }, { 3, 3 }, { 5, 9 }, { 0, 7 }, { 4, 4 }, { 2, 1 },
    });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;

  Solution solution(problem);
  solution.startDate = {
     0,
     0,  1,  4, 10, 17, 20,
    10, 18, 26, 36, 46, 56,
     1,  6, 20, 28, 37, 38,
    18, 23, 28, 33, 45, 53,
     6, 23, 53, 62, 66, 69,
     0,  3,  6, 15, 58, 62,
    70,
  };
  solution.endDate = {
     0,
     1,  4, 10, 17, 20, 26,
    18, 23, 36, 46, 56, 60,
     6, 10, 28, 37, 38, 45,
    23, 28, 33, 36, 53, 62,
    15, 26, 58, 66, 69, 70,
     3,  6, 15, 22, 62, 63,
    70,
  };
  solution.macParent = {
    Or,
    Or, Or, 31, 14, 33, Or,
     3, 25,  6, 15, 16, 22,
     1,  7,  5, 20, 26,  9,
     7, 34,  8,  4, 18, 10,
    13, 19, 23, 24, 11, 12,
    Or, Or, Or,  2, 27, 21,
    30,
  };
  solution.macChild = {
     1,
    13, 34,  7, 22, 15,  9,
    19, 21, 18, 24, 29, 30,
    25,  4, 10, 11, Fn, 23,
    26, 16, 36, 12, 27, 28,
     8, 17, 35, Fn, Fn, Fn,
     3, 14,  5, 20, Fn, Fn,
    Fn,
  };
  solution.isCritMachine = {
    0,
    0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0,
    1, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 1, 0,
    1,
  };
  solution.criticalOp = 30;
  solution.makespan = 70;

  LaarhovenSearch local_search(problem);
  local_search(solution);
}

} // namespace
