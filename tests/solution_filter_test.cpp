#include <limits>
#include "gtest/gtest.h"

#include "data/problem.h"
#include "data/solution.h"
#include "miner/median_filter.h"

namespace {

TEST(SolutionFilterTest, MedianFilter_Should_Return_Best_Solution)
{
  Problem problem(1, 1, 0,
    std::vector<std::pair<MachineId, int>> { { 0, 0 } });

  const OperationId Or = problem.OriginOp;
  const OperationId Fn = problem.FinalOp;
  const int Mn = std::numeric_limits<int>::min();
  const int Mx = std::numeric_limits<int>::max();

  vector<Solution> solution_set;
  vector<int> startDate = { Mn, 0, 0, };
  vector<int> endDate = { 0, 0, Mx, };
  vector<OperationId> macParent = { Or, Or, 1, };
  vector<OperationId> macChild = { 1, Fn, Fn, };
  for (size_t i = 0; i < 3; ++i)
  {
    solution_set.emplace_back(problem);
    solution_set[i].Initialize(
        vector<int>{startDate}
      , vector<int>{endDate}
      , vector<OperationId>{macParent}
      , vector<OperationId>{macChild}
    );
  }

  const double threshold = 0.4; // only retrieve one filtered solution
  MedianFilter solution_filter(threshold);
  auto& filtered_set =  solution_filter(solution_set);

  EXPECT_EQ(filtered_set.size(), 1);
  EXPECT_EQ(filtered_set[0].Makespan(), 0);
}

}
