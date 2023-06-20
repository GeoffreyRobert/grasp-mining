#include "data/solution.h"
#include "empty_miner.h"

extern "C" {
  #include "lcm_max.h"
}

void EmptyMiner::operator()(const vector<Solution>& solutions) {
  std::vector<int> transactions;
  for (const Solution& solution : solutions)
  {
    for (JobId i = 0; i < 1; ++i)
    {
    }
    (void)solution;
  }
}

vector<int> EmptyMiner::ProposePattern(OperationId operation, double support) {
  (void)operation;
  (void)support;
	return vector<int>();
}
