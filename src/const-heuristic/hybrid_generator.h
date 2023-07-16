#ifndef HYBRID_GENERATOR_
#define HYBRID_GENERATOR_

#include <utility>

#include "data/typedefs.h"
#include "const-heuristic/candidate_generator.h"

using std::pair;

class DataMiner;

class HybridGenerator : public CandidateGenerator
{
public:
  HybridGenerator(const Problem&, DataMiner&);
  void Init() override;
  const vector<CandidateJob>& operator()(Solution&) override;
  void IncrementJob(size_t job_idx) override;

private:
  void Write(std::ostream& stream) const override;
  DataMiner& _data_miner;
  // lookup table indexed by operation id and containing
  // following operation in pattern, used to cache itemset
  vector<OperationId> _itemset;
  // lookup table indexed by machine id and containing ops
  // that were scheduled as they belong to a mined pattern
  vector<OperationId> _priority_ops;

  // diagnostics
  unsigned _penalty_count = 0;
  unsigned _schedule_count = 0;
};

#endif // HYBRID_GENERATOR_
