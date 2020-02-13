#include <numeric>

#include "random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement(const Problem& problem) :
	ConstHeuristic(problem), generator(std::random_device()())
{
}

Solution& RandomPlacement::operator()(Solution& solution) {
	int idx, jid, oid, mid;				// index des operations, job ID, operation ID, mach. ID

	// Gestion des contraintes de d�pendance
	vector<int> last_op_on_mac(ref_pb.nMac, -1);	// derni�re op. trait�e par mach.
	vector<int> num_ops_of_job(ref_pb.nJob, 0);	// nombre d'op. trait�es par job
	vector<int> jobs_left(ref_pb.nJob);			// jobs restant � ajouter � la sol.
	iota(jobs_left.begin(), jobs_left.end(), 0);

	int date, date_disj, parent, parent_disj;
	bool is_on_mac;

	for (idx = 0; idx < ref_pb.size; ++idx) {
		jid = ChooseRandomJob(jobs_left);			// generation d'un entier al�atoire

		// r�cup�ration des identifiants operation, machine et parent
		oid = ref_pb.operationNumber[jid][num_ops_of_job[jid]];
		mid = ref_pb.machineNumber[oid];

		// initialisations dur�es+parent
		date = 0; parent = -1; parent_disj = -1; date_disj = 0;

		if (num_ops_of_job[jid] != 0) {		// parent et date hors debut de job
			parent = ref_pb.prevOperation[oid];
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
		solution.AddOperation(oid, date, date + ref_pb.timeOnMachine[oid],
			last_op_on_mac[mid], is_on_mac);

		// stockage des d�pendances
		last_op_on_mac[mid] = oid;
		num_ops_of_job[jid] ++;
		if (num_ops_of_job[jid] == ref_pb.nMac) {
			jobs_left[jid] = jobs_left.back();			// on recup�re le nombre de machines d'un job non termin�
			jobs_left.pop_back();
		}
	}
	return solution;
}

int RandomPlacement::ChooseRandomJob(vector<int>& jobs_left) {
	std::uniform_int_distribution<int> uni(0, static_cast<int>(jobs_left.size()));
	return (int)jobs_left[uni(generator)];
}