#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/candidate_generator.h"
#include "const-heuristic/candidate_selector.h"

ConstHeuristic::ConstHeuristic(
    const Problem& problem
  , CandidateGenerator& generator
  , CandidateSelector& selector)
  : SolverModule(problem)
  , _generator(generator)
  , _selector(selector)
{}

Solution& ConstHeuristic::operator()(Solution& solution)
{
  _generator.Init();

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) {
    auto& candidate_jobs = _generator(solution);
    size_t job_idx = _selector(candidate_jobs);

    // construction de la solution
    auto& c_job = candidate_jobs[job_idx];
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    solution.AddOperation(oid);

    _generator.IncrementJob(job_idx);
  }

  return solution;
}
