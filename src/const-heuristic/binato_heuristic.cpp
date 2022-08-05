#include <utility>

#include "binato_heuristic.h"

using std::pair;
using std::vector;

BinatoHeuristic::BinatoHeuristic(const Problem& problem, double alpha)
  : ConstHeuristic(problem)
  , _alpha(alpha)
  , generator(std::random_device()())
{
  // Gestion des contraintes de dépendance
  last_op_on_mac.resize(problem.nMac, -1); // dernière op. traitée par mach.
  num_ops_of_job.resize(problem.nJob, 0); // nombre d'op. traitées par job

  // Gestion de la Restricted Candidate List
  rc_list.resize(problem.nJob); // Restricted Candidate List

  // Gestion des candidats à la RCL, de leur parent et du makespan
  candidate_jobs.resize(problem.nJob);
  tmp_parent_list.resize(problem.nJob);
  tmp_mkspan_list.resize(problem.nJob);
  tmp_is_on_mac.resize(problem.nJob, false);
}

Solution& BinatoHeuristic::operator()(Solution& solution)
{
  for (unsigned idx = 0; idx < ref_pb.size; ++idx) { // pour chaque operation
    int min_makespan = std::numeric_limits<int>::max();
    int max_makespan = 0;

    // reinitialisation pour stockage des données
    for (unsigned jid = 0; jid < ref_pb.nJob; ++jid) { // pour chaque job

      if (num_ops_of_job[jid] < ref_pb.nMac) { // on ne prend pas les jobs terminés
        unsigned oid = ref_pb.operationNumber[jid][num_ops_of_job[jid]];
        unsigned mid = ref_pb.machineNumber[oid];

        // initialisations durées+parent
        int parent = -1;
        int date = 0;
        if (num_ops_of_job[jid] != 0) { // parent et date hors debut de job
          parent = ref_pb.prevOperation[oid];
          date = solution.endDate[parent];
        }

        int parent_mac = -1;
        int date_disj = 0;
        if (last_op_on_mac[mid] != -1) { // recuperation parent et date disj.
          parent_mac = last_op_on_mac[mid];
          date_disj = solution.endDate[parent_mac];
        }

        if (date < date_disj) { // si la date disj est superieure
          date = date_disj; // on retient date disj
          parent = parent_mac; // on retient le parent
          tmp_is_on_mac[jid] = true;
        } else {
          tmp_is_on_mac[jid] = false;
        }

        // parent et date de fin de l'operation
        tmp_parent_list[jid] = parent;
        tmp_mkspan_list[jid] = date + ref_pb.timeOnMachine[oid];
        candidate_jobs.push_back(jid);

        if (min_makespan > tmp_mkspan_list[jid]) {
          min_makespan = tmp_mkspan_list[jid];
        }

        if (max_makespan < tmp_mkspan_list[jid]) {
          max_makespan = tmp_mkspan_list[jid];
        }
      }
    }

    // on choisit une zone de coupe par rapport au paramètre alpha
    int split_value = min_makespan + static_cast<int>(_alpha * static_cast<double>(max_makespan - min_makespan));

    // construction de la RCL à partir de la splitValue précédente
    for (unsigned c_job : candidate_jobs) {
      if (tmp_mkspan_list[c_job] <= split_value) {
        rc_list.push_back(c_job);
      }
    }

    // choix d'un job aléatoirement dans la RCL
    std::uniform_int_distribution<int> uni(0, static_cast<int>(rc_list.size()) - 1);
    unsigned chosen_job = rc_list[uni(generator)];

    // récupération des identifiants operation, machine et parent
    unsigned oid = ref_pb.operationNumber[chosen_job][num_ops_of_job[chosen_job]];
    unsigned mid = ref_pb.machineNumber[oid];

    // construction de la solution
    int date = 0;
    if (tmp_parent_list[chosen_job] != -1) {
      date = solution.endDate[tmp_parent_list[chosen_job]];
    }
    solution.AddOperation(oid, date, tmp_mkspan_list[chosen_job], last_op_on_mac[mid],
      tmp_is_on_mac[chosen_job]);

    // stockage des dépendances
    last_op_on_mac[mid] = oid;
    ++num_ops_of_job[chosen_job];

    // reinitialisation des structures de données
    candidate_jobs.clear();
    rc_list.clear();
  }
  return solution;
}
