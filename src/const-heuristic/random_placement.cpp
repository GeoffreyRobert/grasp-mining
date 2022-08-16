#include <numeric>

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement(const Problem& problem, unsigned seed)
  : CandidateHeuristic<CandidateJob>(problem, seed)
{
}

Solution& RandomPlacement::operator()(Solution& solution)
{
  Init();

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) {
    // random choice of a job
    unsigned long job_idx = candidate_jobs.size() - 1;
    if (job_idx != 0) {
      job_idx = std::uniform_int_distribution<unsigned long>(0, job_idx)(generator);
    }
    auto& c_job = candidate_jobs[job_idx];

    // récupération des identifiants operation, machine et parent
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];

    // construction de la solution
    solution.GetOperationScheduling(oid);
    solution.AddOperation(oid);

    // increment operation rank and remove job from list if all ops scheduled
    if (++c_job.rank == ref_pb.nMac) {
      c_job = candidate_jobs.back();
      candidate_jobs.pop_back();
    }
  }

  return solution;
}
