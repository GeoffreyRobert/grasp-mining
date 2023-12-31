#ifndef EMPTY_MINER_H_
#define EMPTY_MINER_H_

#include "data_miner.h"

class EmptyMiner : public DataMiner
{
public:
  using DataMiner::DataMiner;
  void operator()(vector<Solution>&) override;
  const vector<pair<OperationId, OperationId>>& GetItemset() override;
  void SetItemset(vector<pair<OperationId, OperationId>>&&);

private:
  vector<pair<OperationId, OperationId>> _dummy_itemset;
};
#endif // EMPTY_MINER_H_
