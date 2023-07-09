#include <cmath>

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/restricted_selector.h"

using std::pair;
using std::vector;

RestrictedSelector::RestrictedSelector(
    const Problem& problem
  , double alpha, unsigned seed)
  : CandidateSelector(problem, seed)
  , _alpha(alpha)
  , rc_list(problem.nJob)
{}


size_t RestrictedSelector::operator()(const vector<CandidateJob>& candidate_jobs)
{
  // data structure reinitialization
  rc_list.clear();

  int min_makespan = std::numeric_limits<int>::max();
  int max_makespan = 0;
  for (auto& c_job : candidate_jobs)
  {
    min_makespan = std::min(min_makespan, c_job.makespan);
    max_makespan = std::max(max_makespan, c_job.makespan);
  }

  // the cutoff is defined from min/max makespans and the alpha parameter
  int cutoff = min_makespan + static_cast<int>(
      std::lround(_alpha * static_cast<double>(max_makespan - min_makespan)));

  // building the RCL from candidate jobs satisfying the cutoff
  size_t job_idx = 0;
  for (auto& c_job : candidate_jobs)
  {
    if (c_job.makespan <= cutoff)
      rc_list.push_back(job_idx);
    ++job_idx;
  }

  // random choice of a candidate in the RCL if it contains more than one element
  size_t rcl_idx = rc_list.size() - 1;
  if (rcl_idx != 0)
    rcl_idx = std::uniform_int_distribution<size_t>(0, rcl_idx)(_r_generator);
  job_idx = rc_list[rcl_idx];

  return job_idx;
}
