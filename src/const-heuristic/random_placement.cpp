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
  auto& candidate_jobs = CandidatesInitialization();

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) {
    auto& c_job = CandidateSelection(candidate_jobs, solution);

    // construction de la solution
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    solution.AddOperation(oid);

    // increment operation rank and remove job from list if all ops scheduled
    if (++c_job.rank == ref_pb.nMac) {
      c_job = candidate_jobs.back();
      candidate_jobs.pop_back();
    }
  }

  return solution;
}

CandidateJob& RandomPlacement::CandidateSelection(vector<CandidateJob>& candidate_jobs, Solution&)
{
  // random choice of a job
  size_t job_idx = candidate_jobs.size() - 1;
  if (job_idx != 0) {
    job_idx = std::uniform_int_distribution<unsigned long>(0, job_idx)(generator);
  }
  return candidate_jobs[job_idx];
}
