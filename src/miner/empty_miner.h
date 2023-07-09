#ifndef EMPTY_MINER_H_
#define EMPTY_MINER_H_

#include "data_miner.h"

class EmptyMiner : public DataMiner {
public:
  using DataMiner::DataMiner;
  void operator()(const vector<Solution>&) override;
  vector<vector<pair<OperationId, OperationId>>> GetItemsets() override;
};
#endif // EMPTY_MINER_H_
