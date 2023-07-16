#include "const-heuristic/random_selector.h"
#include "const-heuristic/const_heuristic.h"

size_t RandomSelector::operator()(const vector<CandidateJob>& candidate_jobs)
{
  // random choice of a job
  size_t job_idx = candidate_jobs.size() - 1;
  if (job_idx != 0) {
    job_idx = std::uniform_int_distribution<unsigned long>(0, job_idx)(_r_generator);
  }
  return job_idx;
}

void RandomSelector::Write(std::ostream& stream) const
{
  stream << "RandomSelector\n";
}
