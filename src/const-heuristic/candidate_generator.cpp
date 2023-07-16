#include <limits>

#include "candidate_generator.h"
#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"

CandidateGenerator::CandidateGenerator(const Problem& problem)
  : SolverModule(problem)
  , _candidate_jobs(ref_pb.nJob)
{}

void CandidateGenerator::Init()
{
  // Gestion des contraintes de dépendance
  _candidate_jobs.clear();
  for (JobId jid = 0; jid < ref_pb.nJob; ++jid)
  {
    _candidate_jobs.push_back({jid, 0, std::numeric_limits<int>::max()});
  }
}

const vector<CandidateJob>& CandidateGenerator::operator()(Solution&)
{
  return _candidate_jobs;
}

void CandidateGenerator::IncrementJob(size_t job_idx)
{
  // increment operation rank and remove job from list if all ops scheduled
  auto& job = _candidate_jobs[job_idx];
  if (++job.rank == ref_pb.nMac)
  {
    job = _candidate_jobs.back();
    _candidate_jobs.pop_back();
  }
}

void CandidateGenerator::Write(std::ostream& stream) const
{
  stream << "CandidateGenerator";
}

std::ostream& operator<<(std::ostream& stream, const CandidateGenerator& generator)
{
  generator.Write(stream);
  return stream;
}
