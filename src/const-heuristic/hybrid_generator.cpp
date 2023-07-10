#include "const-heuristic/hybrid_generator.h"
#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"

HybridGenerator::HybridGenerator(const Problem& problem, DataMiner& data_miner)
  : CandidateGenerator(problem)
  , _data_miner(data_miner)
  , _itemset()
{}

void HybridGenerator::Init()
{
  CandidateGenerator::Init();
  std::fill(_itemset.begin(), _itemset.end(), ref_pb.OriginOp);
  for (auto o_pair : _data_miner.GetItemset())
    _itemset[o_pair.first] = o_pair.second;
}

const vector<CandidateJob>& HybridGenerator::operator()(Solution&)
{
  return _candidate_jobs;
}
