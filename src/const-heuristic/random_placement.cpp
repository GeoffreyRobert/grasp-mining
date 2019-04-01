#include <numeric>

#include "random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement() :
	generator(std::random_device()())
{
}

Solution& RandomPlacement::operator()(const Problem& problem, Solution& solution) {
	int idx, jid, oid, mid;				// index des operations, job ID, operation ID, mach. ID

	// Gestion des contraintes de dépendance
	vector<int> last_op_on_mac(problem.nMac, -1);	// dernière op. traitée par mach.
	vector<int> num_ops_of_job(problem.nJob, 0);	// nombre d'op. traitées par job
	vector<int> jobs_left(problem.nJob);			// jobs restant à ajouter à la sol.
	iota(jobs_left.begin(), jobs_left.end(), 0);

	int date, date_disj, parent, parent_disj;
	bool is_on_mac;

	for (idx = 0; idx < problem.size; ++idx) {
		jid = ChooseRandomJob(jobs_left);			// generation d'un entier aléatoire

		// récupération des identifiants operation, machine et parent
		oid = problem.operationNumber[jid][num_ops_of_job[jid]];
		mid = problem.machineNumber[oid];

		// initialisations durées+parent
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
			is_on_mac = true;
		}
		else {
			is_on_mac = false;
		}

		// construction de la solution
		solution.AddOperation(oid, date, date + problem.timeOnMachine[oid],
			last_op_on_mac[mid], is_on_mac);

		// stockage des dépendances
		last_op_on_mac[mid] = oid;
		num_ops_of_job[jid] ++;
		if (num_ops_of_job[jid] == problem.nMac) {
			jobs_left[jid] = jobs_left.back();			// on recupère le nombre de machines d'un job non terminé
			jobs_left.pop_back();
		}
	}
	return solution;
}

int RandomPlacement::ChooseRandomJob(vector<int>& jobs_left) {
	std::uniform_int_distribution<int> uni(0, jobs_left.size());
	return (int)jobs_left[uni(generator)];
}