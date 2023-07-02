#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "const-heuristic/binato_heuristic.h"

template<class ConstData>
CandidateHeuristic<ConstData>::CandidateHeuristic(const Problem& problem, unsigned seed)
  : ConstHeuristic(problem)
  , generator(seed)
  , _candidate_jobs(ref_pb.nJob)
{}

template<class ConstData>
std::vector<ConstData>& CandidateHeuristic<ConstData>::CandidatesInitialization()
{
  // Gestion des contraintes de dépendance
  _candidate_jobs.clear();
  for (JobId jid = 0; jid < ref_pb.nJob; ++jid) {
    _candidate_jobs.push_back({jid, 0});
  }
  return _candidate_jobs;
}

template class CandidateHeuristic<CandidateJob>;
template class CandidateHeuristic<BinCandidateJob>;
