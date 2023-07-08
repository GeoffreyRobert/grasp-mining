#include "candidate_generator.h"
#include "data/problem.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/binato_heuristic.h"

template<class ConstData>
CandidateGenerator<ConstData>::CandidateGenerator(const Problem& problem)
  : SolverModule(problem)
{}

template<class ConstData>
std::vector<ConstData>& CandidateGenerator<ConstData>::Init()
{
  // Gestion des contraintes de dépendance
  _candidate_jobs.clear();
  for (JobId jid = 0; jid < ref_pb.nJob; ++jid) {
    _candidate_jobs.push_back({jid, 0});
  }
  return _candidate_jobs;
}

template class CandidateGenerator<CandidateJob>;
template class CandidateGenerator<BinCandidateJob>;
