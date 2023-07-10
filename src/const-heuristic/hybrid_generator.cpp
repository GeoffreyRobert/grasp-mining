#include "const-heuristic/hybrid_generator.h"
#include "const-heuristic/const_heuristic.h"

HybridGenerator::HybridGenerator(const Problem& problem, DataMiner& data_miner)
  : CandidateGenerator(problem)
  , _data_miner(data_miner)
{}

void HybridGenerator::Init()
{
  CandidateGenerator::Init();
  _itemsets = _data_miner.GetItemsets();
}

const vector<CandidateJob>& HybridGenerator::operator()(Solution&)
{
  return _candidate_jobs;
}
