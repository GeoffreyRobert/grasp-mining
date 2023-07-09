#include "data/solution.h"
#include "empty_miner.h"

void EmptyMiner::operator()(const vector<Solution>& solutions) {
  (void)solutions;
}

vector<vector<pair<OperationId, OperationId>>> EmptyMiner::GetItemsets()
{
  return {};
}
