#include <limits>

#include "candidate_generator.h"
#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"

std::vector<CandidateJob>& CandidateGenerator::Init()
{
  // Gestion des contraintes de dépendance
  _candidate_jobs.clear();
  for (JobId jid = 0; jid < ref_pb.nJob; ++jid)
  {
    _candidate_jobs.push_back({jid, 0, std::numeric_limits<int>::max()});
  }
  return _candidate_jobs;
}

void CandidateGenerator::IncrementJob(CandidateJob& job)
{
  // increment operation rank and remove job from list if all ops scheduled
  if (++job.rank == ref_pb.nMac)
  {
    job = _candidate_jobs.back();
    _candidate_jobs.pop_back();
  }
}
