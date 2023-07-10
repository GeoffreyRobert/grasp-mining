#include "data/solution.h"
#include "empty_miner.h"

void EmptyMiner::operator()(const vector<Solution>& solutions) {
  (void)solutions;
}

const vector<pair<OperationId, OperationId>>& EmptyMiner::GetItemset()
{
  return _dummy_itemset;
}
