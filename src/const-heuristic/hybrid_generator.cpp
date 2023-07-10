#include "const-heuristic/hybrid_generator.h"
#include "data/problem.h"
#include "data/solution.h"
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

void HybridGenerator::IncrementJob(size_t job_idx)
{
  auto& job = _candidate_jobs[job_idx];

  // remove operation from waiting list if it belonged to it
  OperationId oid = ref_pb.operationNumber[job.jid][job.rank];
  MachineId mid = ref_pb.machineNumber[oid];
  if (oid == _pattern_ops[mid])
    _pattern_ops[mid] = ref_pb.OriginOp;

  // add successor if scheduled operation is part of pattern
  if (_itemset[oid] != ref_pb.OriginOp)
    _pattern_ops[mid] = _itemset[oid];

  // perform the classic job increment
  CandidateGenerator::IncrementJob(job_idx);
}
