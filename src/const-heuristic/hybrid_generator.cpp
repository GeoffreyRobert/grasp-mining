#include "const-heuristic/hybrid_generator.h"
#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"

HybridGenerator::HybridGenerator(const Problem& problem, DataMiner& data_miner)
  : CandidateGenerator(problem)
  , _data_miner(data_miner)
  , _itemset(ref_pb.size)
  , _priority_ops(ref_pb.nMac)
{}

void HybridGenerator::Init()
{
  CandidateGenerator::Init();
  // initialize the waiting list
  std::fill(_priority_ops.begin(), _priority_ops.end(), ref_pb.OriginOp);
  // initialize the lookup table for the mined itemset
  std::fill(_itemset.begin(), _itemset.end(), ref_pb.OriginOp);
  for (auto [oid, next_oid] : _data_miner.GetItemset())
  {
    // directly fill the waiting list with successors of origin op
    if (oid == ref_pb.OriginOp)
    {
      MachineId mid = ref_pb.machineNumber[oid];
      _priority_ops[mid] = next_oid;
    }
    // otherwise add it to the lookup table
    else
    {
      _itemset[oid] = next_oid;
    }
  }
}

const vector<CandidateJob>& HybridGenerator::operator()(Solution& solution)
{
  for (auto& c_job : _candidate_jobs)
  {
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    int makespan = solution.ScheduleOperation(oid);

    // if an operation is scheduled on a machine that already has
    // an operation from a pattern waiting:
    // - if the scheduled op can fit before the waiting op, do nothing
    // - else, treat the operation as if scheduled after the waiting op
    MachineId mid = ref_pb.machineNumber[oid];
    OperationId prio_oid = _priority_ops[mid];
    if (prio_oid != ref_pb.OriginOp
        && oid != prio_oid
        && solution.StartDate(prio_oid) < makespan
        && solution.StartDate(oid) < solution.EndDate(prio_oid))
      makespan = solution.EndDate(prio_oid) + ref_pb.timeOnMachine[oid];
    c_job.makespan = makespan;
  }
  return _candidate_jobs;
}

void HybridGenerator::IncrementJob(size_t job_idx)
{
  auto& job = _candidate_jobs[job_idx];

  // remove operation from waiting list if it belonged to it
  OperationId oid = ref_pb.operationNumber[job.jid][job.rank];
  MachineId mid = ref_pb.machineNumber[oid];
  if (oid == _priority_ops[mid])
    _priority_ops[mid] = ref_pb.OriginOp;

  // add successor if scheduled operation is part of pattern
  if (_itemset[oid] != ref_pb.OriginOp)
    _priority_ops[mid] = _itemset[oid];

  // perform the classic job increment
  CandidateGenerator::IncrementJob(job_idx);
}
