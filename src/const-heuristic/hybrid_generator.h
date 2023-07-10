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

private:
  DataMiner& _data_miner;
  // lookup table indexed by operation id and containing
  // following operation in pattern, used to cache itemset
  vector<OperationId> _itemset;
  // lookup table indexed by machine id and containing ops
  // that were scheduled as they belong to a mined pattern
  vector<OperationId> _pattern_ops;
};

#endif // HYBRID_GENERATOR_
