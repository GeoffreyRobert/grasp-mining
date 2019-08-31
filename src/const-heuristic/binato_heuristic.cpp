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

void BinatoHeuristic::ResourcesAlloc(const Problem&) {

}

Solution& BinatoHeuristic::operator()(const Problem& problem, Solution& solution) {
	int idx, jid, oid, mid;				// index des operations, job ID, operation ID, mach. ID

	// Gestion des contraintes de d�pendance
	vector<int> last_op_on_mac(problem.nMac, -1);	// derni�re op. trait�e par mach.
	vector<int> num_ops_of_job(problem.nJob, 0);	// nombre d'op. trait�es par job

	// Gestion de la Restricted Candidate List
	vector<int> rc_list(problem.nJob);				// Restricted Candidate List
	int chosen_job;									// index et job tir� de la RCL

	// Gestion des candidats � la RCL, de leur parent et du makespan
	int parent, parent_disj, date, date_disj;
	vector<int> candidate_jobs(problem.nJob);
	vector<int> tmp_parent_list(problem.nJob);
	vector<int> tmp_mkspan_list(problem.nJob);
	vector<bool> tmp_is_on_mac(problem.nJob, false);

	// Gestion de la distribution des makespans
	int min_makespan, max_makespan, split_value;

	for (idx = 0; idx < problem.size; ++idx) {		// pour chaque operation
		min_makespan = std::numeric_limits<int>::max();
		max_makespan = 0;

		// reinitialisation pour stockage des donn�es
		for (jid = 0; jid < problem.nJob; ++jid) {		// pour chaque job 

			if (num_ops_of_job[jid] < problem.nMac) {	// on ne prend pas les jobs termin�s
				oid = problem.operationNumber[jid][num_ops_of_job[jid]];
				mid = problem.machineNumber[oid];

				// initialisations dur�es+parent
				date = 0; parent = -1; parent_disj = -1; date_disj = 0;

				if (num_ops_of_job[jid] != 0) {		// parent et date hors debut de job
					parent = problem.prevOperation[oid];
					date = solution.endDate[parent];
				}

				if (last_op_on_mac[mid] != -1) {	// recuperation parent et date disj.			
					parent_disj = last_op_on_mac[mid];
					date_disj = solution.endDate[parent_disj];
				}

				if (date < date_disj) {				// si la date disj est superieure
					date = date_disj;				// on retient date disj
					parent = parent_disj;			// on retient le parent
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

		// on choisit une zone de coupe par rapport au param�tre alpha
		split_value = min_makespan + (int)(alpha * (max_makespan - min_makespan));

		// construction de la RCL � partir de la splitValue pr�c�dente
		for (int c_job : candidate_jobs) {
			if (tmp_mkspan_list[c_job] <= split_value) {
				rc_list.push_back(c_job);
			}
		}

		// choix d'un job al�atoirement dans la RCL
		chosen_job = ChooseRandomJob(rc_list);

		// r�cup�ration des identifiants operation, machine et parent
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

		// stockage des d�pendances
		last_op_on_mac[mid] = oid;
		num_ops_of_job[chosen_job] ++;

		// reinitialisation des structures de donn�es
		candidate_jobs.clear();
		rc_list.clear();
	}
	return solution;
}

int BinatoHeuristic::ChooseRandomJob(const vector<int>& rc_list) {
	std::uniform_int_distribution<int> uni(0, rc_list.size()-1);
	return (int)rc_list[uni(generator)];
}