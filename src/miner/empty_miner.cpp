#include "empty_miner.h"

void EmptyMiner::operator()(const vector<Solution>& solution) {
  (void)solution;
}

vector<int> EmptyMiner::ProposePattern(OperationId operation, double support) {
  (void)operation;
  (void)support;
	return vector<int>();
}
