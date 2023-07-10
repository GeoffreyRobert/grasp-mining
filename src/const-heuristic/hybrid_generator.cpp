#include "const-heuristic/hybrid_generator.h"
#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"

HybridGenerator::HybridGenerator(const Problem& problem, DataMiner& data_miner)
  : CandidateGenerator(problem)
  , _data_miner(data_miner)
  , _itemset(ref_pb.size)
  , _pattern_ops(ref_pb.nMac)
{}

void HybridGenerator::Init()
{
  CandidateGenerator::Init();
  // initialize the waiting list
  std::fill(_pattern_ops.begin(), _pattern_ops.end(), ref_pb.OriginOp);
  // initialize the lookup table for the mined itemset
  std::fill(_itemset.begin(), _itemset.end(), ref_pb.OriginOp);
  for (auto [oid, next_oid] : _data_miner.GetItemset())
  {
    // directly fill the waiting list with successors of origin op
    if (oid == ref_pb.OriginOp)
    {
      MachineId mid = ref_pb.machineNumber[oid];
      _pattern_ops[mid] = next_oid;
    }
    // otherwise add it to the lookup table
    else
    {
      _itemset[oid] = next_oid;
    }
  }
}
