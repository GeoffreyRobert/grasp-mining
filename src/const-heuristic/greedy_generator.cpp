#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/greedy_generator.h"

const vector<CandidateJob>& GreedyGenerator::operator()(Solution& solution)
{
  for (auto& c_job : _candidate_jobs)
  {
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    c_job.makespan = solution.ScheduleOperation(oid);
  }
  return _candidate_jobs;
}
