#include <numeric>

#include "random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement(const Problem& problem) :
	ConstHeuristic(problem), generator(std::random_device()())
{
}

Solution& RandomPlacement::operator()(Solution& solution) {
	// Gestion des contraintes de dépendance
	vector<OperationId> last_op_on_mac(ref_pb.nMac, Problem::InvalidOp);	// dernière op. traitée par mach.
	vector<JobId> num_ops_of_job(ref_pb.nJob, 0);	// nombre d'op. traitées par job
	vector<JobId> jobs_left(ref_pb.nJob);			// jobs restant à ajouter à la sol.
	iota(jobs_left.begin(), jobs_left.end(), 0);

	for (OperationId counter = 0; counter < ref_pb.size; ++counter) {
		JobId jid = ChooseRandomJob(jobs_left);			// generation d'un entier aléatoire

		// récupération des identifiants operation, machine et parent
		OperationId oid = ref_pb.operationNumber[jid][num_ops_of_job[jid]];
		MachineId mid = ref_pb.machineNumber[oid];

		// initialisations durées+parent
    OperationId parent = Problem::InvalidOp;
		int date = 0;
		if (num_ops_of_job[jid] != 0) {		// parent et date hors debut de job
			parent = ref_pb.prevOperation[oid];
			date = solution.endDate[parent];
		}

    OperationId parent_disj = Problem::InvalidOp;
    int date_disj = 0;
		if (last_op_on_mac[mid] != Problem::InvalidOp) {	// recuperation parent et date disj.
			parent_disj = last_op_on_mac[mid];
			date_disj = solution.endDate[parent_disj];
		}

    bool is_on_mac;
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

		// stockage des dépendances
		last_op_on_mac[mid] = oid;
		num_ops_of_job[jid] ++;
		if (num_ops_of_job[jid] == ref_pb.nMac) {
			jobs_left[jid] = jobs_left.back();			// on recupère le nombre de machines d'un job non terminé
			jobs_left.pop_back();
		}
	}
	return solution;
}

JobId RandomPlacement::ChooseRandomJob(const vector<JobId>& jobs_left) {
	std::uniform_int_distribution<unsigned long> uni(0, jobs_left.size());
	return jobs_left[uni(generator)];
}
