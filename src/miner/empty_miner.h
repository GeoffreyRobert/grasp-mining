#ifndef EMPTY_MINER_H_
#define EMPTY_MINER_H_

#include "data_miner.h"

class EmptyMiner : public DataMiner
{
public:
  using DataMiner::DataMiner;
  void operator()(const vector<Solution>&) override;
  const vector<pair<OperationId, OperationId>>& GetItemset() override;

private:
  vector<pair<OperationId, OperationId>> _dummy_itemset;
};
#endif // EMPTY_MINER_H_
