#include "data/solution.h"
#include "empty_miner.h"

void EmptyMiner::operator()(vector<Solution>& solutions) {
  (void)solutions;
}

const vector<pair<OperationId, OperationId>>& EmptyMiner::GetItemset()
{
  return _dummy_itemset;
}

void EmptyMiner::SetItemset(vector<pair<OperationId, OperationId>>&& itemset)
{
  _dummy_itemset = std::move(itemset);
}
