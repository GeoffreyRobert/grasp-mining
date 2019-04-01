#include "data/bierwirth_sequence.h"
#include <numeric>
									
/*
	Fichier incluant diff�rentes m�thodes de construction d'un vecteur de Bierwirth
*/

//	****************************************************************************************
//	Construction d'un vecteur al�atoire
//	****************************************************************************************
void BierwirthSequence::GenerateRandomSequence(const Problem& problem) {
	// job_left : tableau des jobs restant � ajouter au vecteur de Bierwirth
	// ops_left : nombre de traitements restants sur le job i
	vector<int> ops_left(problem.nJob, problem.nMac);
    vector<int> job_left(problem.nJob);
	iota(job_left.begin(), job_left.end(), 0);

	for (int i=0, nb_job=problem.nJob, jid; i< problem.nJob; ++i) {
		jid = genrand64_int64() % problem.nJob;		// generation d'un entier al�atoire
		sequenceVec[i] = job_left[jid];				// stockage du job correspondant
		ops_left[jid]--;							// on decremente le nombre de machines pour le job jid
        if (ops_left[jid] == 0) {
			ops_left[jid] = ops_left[nb_job-1];		// on recuparent le nombre de machines d'un job non termin�	
			job_left[jid] = job_left[--nb_job];		// job_left[jid] re�oit un job non totalement trait� et on d�cremente le nombre de jobs restant � traiter
        }	
	}
}


//	****************************************************************************************
//	Heuristique de construction bas�e sur le makespan de l'op�ration ajout�e � chaque �tape
//	****************************************************************************************
void BierwirthSequence::GenerateBinato(const Problem& problem, double alpha) {
	int idx, jid, oid, mid;				// index de Bierwirth, job ID, operation ID, mach. ID

	// Gestion des contraintes de d�pendance
	vector<int> last_op_on_mac(problem.nMac, -1);	// derni�re op. trait�e par mach.
	vector<int> num_ops_on_mac(problem.nMac, 0);	// nombre d'op. trait�es par mach.
	vector<int> num_ops_of_job(problem.nJob, 0);	// nombre d'op. trait�es par job

	// Gestion de la Restricted Candidate List
	vector<int> rc_list(problem.nJob);				// Restricted Candidate List
	int index_job, chosen_job;						// index et job tir� de la RCL

	// Gestion du makespan et des dates d'ex�cution
	int parent, parent_disj, date, date_disj, date_end;
	vector<int> start_date(problem.size, 0);		// date de d�marrage de traitement
	vector<int> end_date(problem.size, 0);			// date de fin de traitement

	// Gestion de la distribution des makespans
	int minMakespan, maxMakespan, split_value;
	vector<pair<int,int>> job_and_makespan(problem.nJob);

	makespan = 0;
	for (idx = 0; idx < problem.size; ++idx) { // pour chaque operation
		minMakespan = std::numeric_limits<int>::max();
		maxMakespan = 0;

		// reinitialisation pour stockage des donn�es
        for (jid = 0; jid < problem.nJob; ++jid) {		// pour chaque job 

            if (num_ops_of_job[jid] < problem.nMac) {	// on ne prend pas les jobs termin�s
				oid = problem.operationNumber[jid][num_ops_of_job[jid]];
				mid = problem.machineNumber[oid];
                // initialisations dur�es+parent
                date = 0; parent = -1; parent_disj = -1; date_disj = 0;
                
                if (num_ops_of_job[jid] != 0) {		// parent et date hors debut de job
                    parent = problem.prevOperation[oid];
					date = end_date[parent];
                }
							
				if (last_op_on_mac[mid] != -1) {	// recuperation parent et date disj.			
                    parent_disj = last_op_on_mac[mid]; 
                    date_disj = end_date[parent_disj];
                }
                
                if (date < date_disj) {				// si la date disj est superieure
                    date = date_disj;				// on retient date disj
                    parent = parent_disj;			// on retient le parent
                } 
                // date de fin de l'operation
				date_end = date + problem.timeOnMachine[oid]; 

				job_and_makespan.push_back(std::make_pair(jid, date_end));
				start_date[jid] = date;
				end_date[jid] = date_end;

				if (minMakespan > date_end) {
					minMakespan = date_end;
				}

				if (maxMakespan < date_end) {
					maxMakespan = date_end;
				}
            }
        }

		// on choisit une zone de coupe par rapport au param�tre alpha
        split_value = minMakespan + (int) (alpha * (maxMakespan-minMakespan));
            
        // construction de la RCL � partir de la splitValue pr�c�dente
		for (pair<int,int> j_and_m: job_and_makespan) {
			if (j_and_m.second <= split_value) {
				rc_list.push_back(j_and_m.first);
			}
		}

        // choix d'un job al�atoirement dans la RCL
		index_job = genrand64_int64() % (rc_list.size() > 0 ? rc_list.size() : 1);
        chosen_job = rc_list[index_job];

        // stockage des donn�es pour l'operation
		sequenceVec[idx] = chosen_job;

		// stockage des d�pendances
		oid = problem.operationNumber[chosen_job][num_ops_of_job[chosen_job]];
		mid = problem.machineNumber[oid];

		last_op_on_mac[mid] = oid;
		num_ops_on_mac[mid] ++;
        num_ops_of_job[chosen_job] ++;

		// reinitialisation des structures de donn�es
		job_and_makespan.clear();
		rc_list.clear();
	} 

}

