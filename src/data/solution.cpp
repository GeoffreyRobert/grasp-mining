#include "solution.h"


Solution::Solution(const Problem& problem) :
	problem_(problem) {
};


/* fonction d'�valuation du vecteur de bierwirth � partir d'un indice donn�
*/
void Solution::Evaluate(BierwirthSequence& sequence, int start=0) {
	vector<int> last_op_on_mac(problem_.nMac, -1);	// derni�re op. trait�e par mach.
	vector<int> num_ops_on_mac(problem_.nMac, 0);	// nombre d'op. trait�es par mach.
	vector<int> num_ops_of_job(problem_.nJob, 0);	// nombre d'op. trait�es par job

	int idx, jid, oid, mid;				// index de Bierwirth, job ID, operation ID, mach. ID
	
	if (start > 0) {					// on red�pile les informations
		for (int idx = 0; idx < start; ++idx) {
			jid = sequence.sequenceVec[idx];
			oid = problem_.operationNumber[jid][num_ops_of_job[jid]];
			mid = problem_.machineNumber[oid];

			last_op_on_mac[mid] = oid;
			num_ops_on_mac[mid] ++;
			num_ops_of_job[jid] ++;
		}
	}

	int rank;							// rang de l'op.
	int parent = 0, parent_disj = -1;	// parent de l'op. dans le job, parent de l'op sur la machine
	int date = 0, date_disj = 0;		// date de d�marrage apr�s parent, """ apr�s parent disjoint

	for (idx = start; idx < problem_.size; ++idx) {
		
		// Informations sur le sommet
		jid = sequence.sequenceVec[idx];			// numero du job de l'op�ration
		rank = num_ops_of_job[jid];					// rang de l'op�ration dans le job
		oid = problem_.operationNumber[jid][rank];	// id de l'op�ration dans le probl�me
		mid = problem_.machineNumber[oid];			// numero de machine qui traite la piece
		parent = problem_.prevOperation[oid];		// num�ro du parent de l'op. dans le job
		parent_disj = last_op_on_mac[mid];			// num�ro du parent de l'op. sur la machine

		if (rank != 0) {							// recuperation date si pas en debut de job
			date = endDate[parent];
		}

		if (last_op_on_mac[mid] != -1) {			// recuperation parent et date Disjonctifs
			date_disj = endDate[parent_disj];
		}

		if (date < date_disj) {						// si la dur�e disj est superieure � horiz
			date = date_disj;						// on retient date disj
			parent = parent_disj;					// on retient le pere
		}

		parentList[oid] = parent;					// parent de l'op�ration
		childList[oid] = INFINITE_C;				// successeur non initialis� de l'operation
		startDate[oid] = date;						// date de d�but de l'operation
		endDate[oid] = date + problem_.timeOnMachine[oid];	//date de fin de l'operation

		if (last_op_on_mac[mid] != -1) {			// insertion de l'operation comme successeure
			childList[last_op_on_mac[mid]] = oid;
		}

		last_op_on_mac[mid] = oid;					// m�j de la derni�re op. pass�e sur la machine
		num_ops_on_mac[mid] ++;						// incr�ment du nombre de fois o� on a vu l'op.
		num_ops_of_job[jid] ++;						// incr�ment du nombre d'ops. trait�es du job

	}

	// Phase de calcul du MAKESPAN
	makespan = 0;
	for (mid=0; mid<problem_.nMac; ++mid) {
		date = endDate[last_op_on_mac[mid]];
		if (date > makespan) {
			makespan = date;
			parentList[problem_.size] = last_op_on_mac[machine];
		}
	}

}