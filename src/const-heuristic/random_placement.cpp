#include <numeric>

#include "const-heuristic/random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement(
    const Problem& problem
  , CandidateGenerator<CandidateJob>& c_generator
  , unsigned seed)
  : CandidateHeuristic<CandidateJob>(problem, c_generator, seed)
{
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
