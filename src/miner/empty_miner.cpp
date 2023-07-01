#include "data/solution.h"
#include "empty_miner.h"

void EmptyMiner::operator()(const vector<Solution>& solutions) {
  (void)solutions;
}

vector<int> EmptyMiner::ProposePattern(OperationId operation, double support) {
  (void)operation;
  (void)support;
	return vector<int>();
}
