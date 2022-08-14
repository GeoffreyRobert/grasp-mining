#include <utility>

#include "binato_heuristic.h"

using std::pair;
using std::vector;

BinatoHeuristic::BinatoHeuristic(const Problem& problem, double alpha)
  : ConstHeuristic(problem)
  , _alpha(alpha)
  , generator(std::random_device()())
  , num_ops_of_job(problem.nJob, 0)
  , rc_list(0)
  , candidate_jobs(problem.nJob)
  , tmp_parent_list(problem.nJob)
  , tmp_mkspan_list(problem.nJob)
  , tmp_is_on_mac(problem.nJob, false)
{
  // the restricted candidate list starts empty but can fill up
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
      if (rank >= ref_pb.nMac) // on ne ignore les jobs terminés
        continue;

      OperationId oid = ref_pb.operationNumber[jid][rank];

      auto [parent, start_date, is_on_mac] = solution.GetOperationScheduling(oid);

      // parent et date de fin de l'operation
      tmp_parent_list[jid] = parent;
      tmp_mkspan_list[jid] = start_date + ref_pb.timeOnMachine[oid];
      tmp_is_on_mac[jid] = is_on_mac;
      candidate_jobs.push_back(jid);

      if (min_makespan > tmp_mkspan_list[jid]) {
        min_makespan = tmp_mkspan_list[jid];
      }

      if (max_makespan < tmp_mkspan_list[jid]) {
        max_makespan = tmp_mkspan_list[jid];
      }
    }

    // on choisit une zone de coupe par rapport au paramètre alpha
    int split_value = min_makespan + static_cast<int>(_alpha * static_cast<double>(max_makespan - min_makespan));

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
    OperationId oid = ref_pb.operationNumber[chosen_job][num_ops_of_job[chosen_job]];

    // construction de la solution
    solution.AddOperation(oid);

    // stockage des dépendances
    ++num_ops_of_job[chosen_job];

    // reinitialisation des structures de données
    candidate_jobs.clear();
    rc_list.clear();
  }
  return solution;
}
