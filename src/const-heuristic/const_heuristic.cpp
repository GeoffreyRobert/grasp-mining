#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "const-heuristic/binato_heuristic.h"

template<class ConstData>
CandidateHeuristic<ConstData>::CandidateHeuristic(const Problem& problem, unsigned seed)
  : ConstHeuristic(problem)
  , generator(seed)
  , candidate_jobs(ref_pb.nJob)
{}

template<class ConstData>
void CandidateHeuristic<ConstData>::Init()
{
  // Gestion des contraintes de dépendance
  candidate_jobs.clear();
  for (JobId jid = 0; jid < ref_pb.nJob; ++jid) {
    candidate_jobs.push_back({jid, 0});
  }
}

template class CandidateHeuristic<CandidateJob>;
template class CandidateHeuristic<BinCandidateJob>;
