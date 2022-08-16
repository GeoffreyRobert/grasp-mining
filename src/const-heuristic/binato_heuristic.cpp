#include <cmath>

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/binato_heuristic.h"

using std::pair;
using std::vector;

BinatoHeuristic::BinatoHeuristic(const Problem& problem, double alpha, unsigned seed)
  : CandidateHeuristic<BinCandidateJob>(problem, seed)
  , _alpha(alpha)
  , rc_list(problem.nJob)
{
}

void BinatoHeuristic::Init()
{
  CandidateHeuristic<BinCandidateJob>::Init();
  rc_list.clear();
}

Solution& BinatoHeuristic::operator()(Solution& solution)
{
  Init();

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) { // pour chaque operation
    int min_makespan = std::numeric_limits<int>::max();
    int max_makespan = 0;

    for (auto& c_job : candidate_jobs) {
      OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
      int end_date = solution.GetOperationScheduling(oid);

      // parent et date de fin de l'operation
      c_job.makespan = end_date;

      if (min_makespan > end_date) {
        min_makespan = end_date;
      }
      if (max_makespan < end_date) {
        max_makespan = end_date;
      }
    }

    // on choisit une zone de coupe par rapport au paramètre alpha
    int offset = static_cast<int>(
        std::lround(_alpha * static_cast<double>(max_makespan - min_makespan)));
    int split_value = min_makespan + offset;

    // construction de la RCL à partir de la splitValue précédente
    for (auto& c_job : candidate_jobs) {
      if (c_job.makespan <= split_value) {
        rc_list.push_back(&c_job);
      }
    }

    // random choice of a candidate in the RCL if contains more than one element
    unsigned long job_idx = rc_list.size() - 1;
    if (job_idx != 0) {
      job_idx = std::uniform_int_distribution<unsigned long>(0, job_idx)(generator);
    }
    auto& c_job = *rc_list[job_idx];

    // construction de la solution
    OperationId oid = ref_pb.operationNumber[c_job.jid][c_job.rank];
    solution.AddOperation(oid);

    // increment operation rank and remove job from list if all ops scheduled
    if (++c_job.rank == ref_pb.nMac) {
      c_job = candidate_jobs.back();
      candidate_jobs.pop_back();
    }

    // reinitialisation des structures de données
    rc_list.clear();
  }

  return solution;
}
