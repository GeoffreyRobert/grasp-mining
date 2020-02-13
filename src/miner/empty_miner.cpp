#include "empty_miner.h"

void EmptyMiner::operator()(const vector<Solution>& solution) {
}

vector<int> EmptyMiner::ProposePattern(int operation, double support) {
	return vector<int>();
}