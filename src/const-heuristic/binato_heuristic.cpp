#include <cmath>

#include "const-heuristic/binato_heuristic.h"

using std::pair;
using std::vector;

BinatoHeuristic::BinatoHeuristic(const Problem& problem, double alpha)
  : ConstHeuristic(problem)
  , _alpha(alpha)
  , generator(std::random_device()())
  , num_ops_of_job(problem.nJob, 0)
  , rc_list(0)
  , tmp_mkspan_list(problem.nJob)
{
  // the restricted candidate list starts empty but can fill up
  candidate_jobs.reserve(problem.nJob);
  rc_list.reserve(problem.nJob);
}

Solution& BinatoHeuristic::operator()(Solution& solution)
{
  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) { // pour chaque operation
    int min_makespan = std::numeric_limits<int>::max();
    int max_makespan = 0;

    // reinitialisation pour stockage des données
    for (JobId jid = 0; jid < ref_pb.nJob; ++jid) { // pour chaque job

      OperationRank rank = num_ops_of_job[jid];
      if (rank >= ref_pb.nMac) // ignore completely scheduled jobs
        continue;

      OperationId oid = ref_pb.operationNumber[jid][rank];
      auto [parent, start_date, is_on_mac] = solution.GetOperationScheduling(oid);

      // parent et date de fin de l'operation
      int tmp_makespan = start_date + ref_pb.timeOnMachine[oid];
      tmp_mkspan_list[jid] = tmp_makespan;
      candidate_jobs.push_back(jid);

      if (min_makespan > tmp_makespan) {
        min_makespan = tmp_makespan;
      }

      if (max_makespan < tmp_makespan) {
        max_makespan = tmp_makespan;
      }
    }

    // on choisit une zone de coupe par rapport au paramètre alpha
    int offset = static_cast<int>(std::lround(_alpha * static_cast<double>(max_makespan - min_makespan)));
    int split_value = min_makespan + offset;

    // construction de la RCL à partir de la splitValue précédente
    for (JobId c_job : candidate_jobs) {
      if (tmp_mkspan_list[c_job] <= split_value) {
        rc_list.push_back(c_job);
      }
    }

    // choix d'un job aléatoirement dans la RCL
    std::uniform_int_distribution<unsigned long> uni(0, rc_list.size()-1);
    JobId chosen_job = rc_list[uni(generator)];

    // récupération des identifiants operation, machine et parent
    OperationRank rank = num_ops_of_job[chosen_job];
    OperationId oid = ref_pb.operationNumber[chosen_job][rank];

    // construction de la solution
    solution.AddOperation(oid);

    // stockage des dépendances
    ++num_ops_of_job[chosen_job];

    // reinitialisation des structures de données
    candidate_jobs.clear();
    rc_list.clear();
  }
  std::fill(num_ops_of_job.begin(), num_ops_of_job.end(), 0);

  return solution;
}
