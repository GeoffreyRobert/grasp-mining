#include "empty_miner.h"

void EmptyMiner::operator()(const Problem& problem, const vector<Solution>& solution) {
}

vector<int> EmptyMiner::ProposePattern(const Problem& problem, int operation, double support) {
	return vector<int>();
}