#ifndef HYBRID_GENERATOR_
#define HYBRID_GENERATOR_

#include <utility>

#include "const-heuristic/candidate_generator.h"
#include "miner/data_miner.h"

using std::pair;

class HybridGenerator : public CandidateGenerator
{
public:
  HybridGenerator(const Problem&, DataMiner&);
  void Init() override;
  const vector<CandidateJob>& operator()(Solution&) override;

private:
  DataMiner& _data_miner;
  vector<vector<std::pair<OperationId, OperationId>>> _itemsets;
};

#endif // HYBRID_GENERATOR_
