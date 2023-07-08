#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/candidate_generator.h"

template<class ConstData>
CandidateHeuristic<ConstData>::CandidateHeuristic(
    const Problem& problem
  , CandidateGenerator<ConstData>& c_generator
  , unsigned seed)
  : ConstHeuristic(problem)
  , generator(seed)
  , _c_generator(c_generator)
{}

template<class ConstData>
Solution& CandidateHeuristic<ConstData>::operator()(Solution& solution)
{
  auto& candidate_jobs = _c_generator.Init();

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) {
    auto& c_job = CandidateSelection(candidate_jobs, solution);

    // construction de la solution
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    solution.AddOperation(oid);

    _c_generator.IncrementJob(c_job);
  }

  return solution;
}

template class CandidateHeuristic<CandidateJob>;
template class CandidateHeuristic<BinCandidateJob>;
