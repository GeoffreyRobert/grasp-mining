#include <utility>

#include "binato_heuristic.h"

using std::vector; using std::pair;

BinatoHeuristic::BinatoHeuristic() :
	generator(std::random_device()())
{
}

BinatoHeuristic::BinatoHeuristic(double alpha) :
	generator(std::random_device()())
{
	this->alpha = alpha;
}

void BinatoHeuristic::ResourcesAlloc(const Problem& problem) {
    // Gestion des contraintes de dépendance
    last_op_on_mac.resize(problem.nMac, -1);	// dernière op. traitée par mach.
    num_ops_of_job.resize(problem.nJob, 0);	// nombre d'op. traitées par job

    // Gestion de la Restricted Candidate List
    rc_list.resize(problem.nJob);				// Restricted Candidate List

    // Gestion des candidats à la RCL, de leur parent et du makespan
    candidate_jobs.resize(problem.nJob);
    tmp_parent_list.resize(problem.nJob);
    tmp_mkspan_list.resize(problem.nJob);
    tmp_is_on_mac.resize(problem.nJob, false);
}

Solution& BinatoHeuristic::operator()(const Problem& problem, Solution& solution) {
	// index des operations, job ID, operation ID, mach. ID
    int jid, oid, mid;

	// Gestion des candidats à la RCL, de leur parent et du makespan
	int parent, parent_mac, date, date_disj;

	// Gestion de la distribution des makespans
	int min_makespan, max_makespan, split_value;
    int chosen_job;									// index et job tiré de la RCL

	for (int idx = 0; idx < problem.size; ++idx) {		// pour chaque operation
		min_makespan = std::numeric_limits<int>::max();
		max_makespan = 0;

		// reinitialisation pour stockage des données
		for (jid = 0; jid < problem.nJob; ++jid) {		// pour chaque job 

			if (num_ops_of_job[jid] < problem.nMac) {	// on ne prend pas les jobs terminés
				oid = problem.operationNumber[jid][num_ops_of_job[jid]];
				mid = problem.machineNumber[oid];

                // initialisations durées+parent
                parent = -1; date = 0;
				if (num_ops_of_job[jid] != 0) {		// parent et date hors debut de job
					parent = problem.prevOperation[oid];
					date = solution.endDate[parent];
				}

                parent_mac = -1; date_disj = 0;
				if (last_op_on_mac[mid] != -1) {	// recuperation parent et date disj.			
					parent_mac = last_op_on_mac[mid];
					date_disj = solution.endDate[parent_mac];
				}

				if (date < date_disj) {				// si la date disj est superieure
					date = date_disj;				// on retient date disj
					parent = parent_mac;			// on retient le parent
					tmp_is_on_mac[jid] = true;
				}
				else {
					tmp_is_on_mac[jid] = false;
				}

				// parent et date de fin de l'operation
				tmp_parent_list[jid] = parent;
				tmp_mkspan_list[jid] = date + problem.timeOnMachine[oid];
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
		split_value = min_makespan + static_cast<int>(alpha * static_cast<double>(max_makespan - min_makespan));

		// construction de la RCL à partir de la splitValue précédente
		for (int c_job : candidate_jobs) {
			if (tmp_mkspan_list[c_job] <= split_value) {
				rc_list.push_back(c_job);
			}
		}

		// choix d'un job aléatoirement dans la RCL
        std::uniform_int_distribution<int> uni(0, rc_list.size() - 1);
        chosen_job = (int)rc_list[uni(generator)];

		// récupération des identifiants operation, machine et parent
		oid = problem.operationNumber[chosen_job][num_ops_of_job[chosen_job]];
		mid = problem.machineNumber[oid];

		// construction de la solution
		if (tmp_parent_list[chosen_job] == -1) {
			date = 0;
		}
		else {
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