#include <cmath>

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/binato_heuristic.h"

using std::pair;
using std::vector;

BinatoHeuristic::BinatoHeuristic(
    const Problem& problem
  , CandidateGenerator<BinCandidateJob>& generator
  , double alpha, unsigned seed)
  : CandidateHeuristic<BinCandidateJob>(problem, generator, seed)
  , _alpha(alpha)
  , rc_list(problem.nJob)
{
}

std::pair<int, int> BinatoHeuristic::CandidatesGeneration(vector<BinCandidateJob>& candidate_jobs, Solution& solution)
{
  int min_makespan = std::numeric_limits<int>::max();
  int max_makespan = 0;

  for (auto& c_job : candidate_jobs) {
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    int end_date = solution.ScheduleOperation(oid);

    // assign a makespan to each candidate job
    c_job.makespan = end_date;

    min_makespan = std::min(min_makespan, end_date);
    max_makespan = std::max(max_makespan, end_date);
  }

  return {min_makespan, max_makespan};
}

BinCandidateJob& BinatoHeuristic::CandidateSelection(vector<BinCandidateJob>& candidate_jobs, Solution& solution)
{
    auto [min_makespan, max_makespan] =
      BinatoHeuristic::CandidatesGeneration(candidate_jobs, solution);

    // data structure reinitialization
    rc_list.clear();

    // the cutoff is defined from min/max makespans and the alpha parameter
    int offset = static_cast<int>(
        std::lround(_alpha * static_cast<double>(max_makespan - min_makespan)));
    int split_value = min_makespan + offset;

    // building the RCL from candidate jobs satisfying the cutoff
    size_t job_idx = 0;
    for (auto& c_job : candidate_jobs) {
      if (c_job.makespan <= split_value) {
        rc_list.push_back(job_idx);
      }
      ++job_idx;
    }

    // random choice of a candidate in the RCL if it contains more than one element
    size_t rcl_idx = rc_list.size() - 1;
    if (rcl_idx != 0) {
      rcl_idx = std::uniform_int_distribution<size_t>(0, rcl_idx)(generator);
    }
    job_idx = rc_list[rcl_idx];
    return candidate_jobs[job_idx];
}
