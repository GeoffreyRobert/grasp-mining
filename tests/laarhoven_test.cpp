#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "data/problem.h"
#include "data/solution.h"
#include "local-search/laarhoven_search.h"

using std::string;

namespace {

static constexpr OperationId Na = Problem::InvalidOp;

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

  Solution solution(problem);
  solution.startDate = {
    0, 1,
    2, 3,
  };
  solution.endDate = {
    1, 2,
    3, 4,
  };
  solution.macParent = {
    Na, Na,
    1, 0,
  };
  solution.macChild = {
    3, 2,
    Na, Na,
  };
  solution.isCritMachine = {
    0, 0,
    1, 0,
  };
  solution.criticalOp = 3;
  solution.makespan = 4;

  const int final_makespan = 2;
  const int final_criticalOp = 1;

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

  Solution solution(problem);
  solution.startDate = {
    0, 1,
    1, 2,
  };
  solution.endDate = {
    1, 2,
    2, 3,
  };
  solution.macParent = {
    Na, Na,
    0, 1,
  };
  solution.macChild = {
    2, 3,
    Na, Na,
  };
  solution.isCritMachine = {
    0, 0,
    1, 1,
  };
  solution.criticalOp = 3;
  solution.makespan = 3;

  const int final_makespan = 3;
  const int final_criticalOp = 3;

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

  Solution solution(problem);
  solution.startDate = {
    0, 1, 2,
    0, 2, 3,
  };
  solution.endDate = {
    1, 2, 3,
    1, 3, 4,
  };
  solution.macParent = {
    Na, Na, 3,
    Na, 1, 0,
  };
  solution.macChild = {
    5, 4, Na,
    2, Na, Na,
  };
  solution.isCritMachine = {
    0, 0, 0,
    0, 1, 0,
  };
  solution.criticalOp = 5;
  solution.makespan = 4;

  const int final_criticalOp = 5;
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

  Solution solution(problem);
  solution.startDate = {
    0, 1,
    0, 2,
    0, 3,
  };
  solution.endDate = {
    1, 2,
    0, 3,
    0, 4,
  };
  solution.macParent = {
    2, Na,
    4,  1,
    Na, 3,
  };
  solution.macChild = {
    Na, 3,
     0, 5,
     2, Na,
  };
  solution.isCritMachine = {
    1, 0,
    1, 1,
    0, 1,
  };
  solution.criticalOp = 5;
  solution.makespan = 4;

  const int final_criticalOp = 5;
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

  Solution solution(problem);
  solution.startDate = {
    0, 1, 3,
    0, 4, 5,
    1, 2, 3,
  };
  solution.endDate = {
    1, 2, 5,
    1, 5, 6,
    2, 3, 4,
  };
  solution.macParent = {
    Na, Na, 7,
    Na,  8, 6,
     0,  3, 1,
  };
  solution.macChild = {
    6,  8, Na,
    7, Na, Na,
    5,  2,  4,
  };
  solution.isCritMachine = {
    0, 0, 1,
    0, 1, 0,
    1, 0, 0,
  };
  solution.criticalOp = 5;
  solution.makespan = 6;

  const int final_criticalOp = 2;
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

  Solution solution(problem);
  solution.startDate = {
    0, 1, 4, 10, 17, 20,
    10, 18, 26, 36, 46, 56,
    1, 6, 20, 28, 37, 38,
    18, 23, 28, 33, 45, 53,
    6, 23, 53, 62, 66, 69,
    0, 3, 6, 15, 58, 62 };
  solution.endDate = {
    1, 4, 10, 17, 20, 26,
    18, 23, 36, 46, 56, 60,
    6, 10, 28, 37, 38, 45,
    23, 28, 33, 36, 53, 62,
    15, 26, 58, 66, 69, 70,
    3, 6, 15, 22, 62, 63 };
  solution.macParent = {
    Na, Na, 30, 13, 32, Na,
    2, 24, 5, 14, 15, 21,
    0, 6, 4, 19, 25, 8,
    6, 33, 7, 3, 17, 9,
    12, 18, 22, 23, 10, 11,
    Na, Na, Na, 1, 26, 20 };
  solution.macChild = {
    12, 33, 6, 21, 14, 8,
    18, 20, 17, 23, 28, 29,
    24, 3, 9, 10, Na, 22,
    25, 15, 35, 11, 26, 27,
    7, 16, 34, Na, Na, Na,
    2, 13, 4, 19, Na, Na };
  solution.isCritMachine = {
    0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0,
    1, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 1, 0 };
  solution.criticalOp = 29;
  solution.makespan = 70;

  LaarhovenSearch local_search(problem);
  local_search(solution);
}

} // namespace
