#include "data/problem.h"
#include "data/solution.h"
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

template<class ConstData>
Solution& CandidateHeuristic<ConstData>::operator()(Solution& solution)
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

template class CandidateHeuristic<CandidateJob>;
template class CandidateHeuristic<BinCandidateJob>;
