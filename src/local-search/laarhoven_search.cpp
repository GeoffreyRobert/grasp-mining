#include <utility>
#include <deque>

#include "laarhoven_search.h"

using std::deque;

LaarhovenSearch::LaarhovenSearch(const Problem& problem) : LocalSearch(problem)
{
	is_changed.resize(ref_pb.size, OpUpdate::Unchanged);
	new_start_date.resize(ref_pb.size);
	new_end_date.resize(ref_pb.size);
	new_is_crit_mac.resize(ref_pb.size);
    ops_to_move.set_capacity(ref_pb.size);
}

Solution& LaarhovenSearch::operator()(Solution& sol) {
	int operation = sol.criticalOp;			// op. considérée pour relocation
	int parent = sol.macParent[operation];	// parent de l'operation

	// remonter le chemin critique
	do {
		if (!sol.isCritMachine[operation]) {
            // on ignore deux ops. dans le meme job
			operation = ref_pb.prevOperation[operation];
		}
		else if (SwapAndEvaluate(sol, parent, operation)) {
            // on inverse deux ops. sur le chemin critique si réduction du makespan
			sol.DoChanges(tmp_critical, tmp_makespan, is_changed,
                new_start_date, new_end_date, new_is_crit_mac);
            std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
			hit_count++;            // TODO: sous preproc. pour debug
			operation = sol.criticalOp;
		}
		else {
            // on continue sur le chemin critique
			operation = parent;
		}
        parent = sol.macParent[operation];
	} while (parent != -1);
	return sol;

}

/** En charge de la sélection des opérations à examiner
**/
bool LaarhovenSearch::SwapAndEvaluate(Solution& sol, int parent, int child)
{
    // cas qui allonge forcément le chemin critique
	if (ref_pb.prevOperation[child] != -1 &&
		sol.startDate[parent] < sol.endDate[ref_pb.prevOperation[child]]) {
		return false;
	}

    // initialisation de l'op. crit. et du makespan
    tmp_critical = sol.criticalOp;
    tmp_makespan = sol.makespan;

    // inversion des 2 opérations sur le chemin critique
    SwapAndUpdateOps(sol, parent, child);

    // initialisation des successeurs à traiter
    if (ref_pb.nextOperation[parent] != -1) {
        ops_to_move.push_back(ref_pb.nextOperation[parent]);
        is_changed[ref_pb.nextOperation[parent]] = OpUpdate::ToChange;
    }
    if (sol.macChild[child] != -1) { // nécessairement child sur le chemin critique
        ops_to_move.push_back(sol.macChild[child]);
        is_changed[sol.macChild[child]] = OpUpdate::ToChange;
    }
    if (ref_pb.nextOperation[child] != -1) {
        ops_to_move.push_back(ref_pb.nextOperation[child]);
        is_changed[ref_pb.nextOperation[child]] = OpUpdate::ToChange;
    }

    unsigned oid;
    // stack vs queue en termes de perfs ? stack: localité, queue: moins de doubles accès
	while (!ops_to_move.empty()) {
		// récupération des identifiants
		oid = ops_to_move.front();
		ops_to_move.pop_front();

        // mise-à-jour des valeurs de l'opération
        UpdateOp(sol, oid);

        // vérification que la nouvelle solution reste sub-critique
        if (ref_pb.nextOperation[oid] == -1 && sol.macChild[oid] == -1) {
            if (new_end_date[oid] >= tmp_makespan) {
                tmp_critical = oid;
                tmp_makespan = new_end_date[oid];
                if (new_end_date[oid] > sol.makespan) { // ">" est un choix heuristique
                    CancelSwap(sol, parent, child);
                    return false;
                }
            }
        }
        AddSuccessors(sol, oid);
	}
	return true;
}


void LaarhovenSearch::SwapAndUpdateOps(Solution& sol, unsigned parent, unsigned child)
{
    // variables de l'algorithme
    unsigned date_job, date_mac, date_par;

    // intialisation des dates pour le successeur
    if (ref_pb.prevOperation[child] != -1) {
        date_job = sol.endDate[ref_pb.prevOperation[child]];
    }
    else {
        date_job = 0;
    }
    if (sol.macParent[parent] != -1) {
        date_mac = sol.endDate[sol.macParent[parent]];
    }
    else {
        date_mac = 0;
    }
    if (date_job < date_mac) {
        date_par = date_mac;
        new_is_crit_mac[child] = true;
    }
    else {
        date_par = date_job;
        new_is_crit_mac[child] = false;
    }
    new_start_date[child] = date_par;
    new_end_date[child] = date_par + ref_pb.timeOnMachine[child];
    is_changed[child] = OpUpdate::Changed;

    // intialisation des dates pour le parent
    if (ref_pb.prevOperation[parent] != -1) {
        date_job = sol.endDate[ref_pb.prevOperation[parent]];
    }
    else {
        date_job = 0;
    }
    date_mac = new_end_date[child];
    if (date_job < date_mac) {
        date_par = date_mac;
        new_is_crit_mac[parent] = true;
    }
    else {
        date_par = date_job;
        new_is_crit_mac[parent] = false;
    }
    new_start_date[parent] = date_par;
    new_end_date[parent] = date_par + ref_pb.timeOnMachine[parent];
    is_changed[parent] = OpUpdate::Changed;

    // inversion parent / successeur dans l'ordre sur les machines
    sol.SwapOperations(parent, child);
}


void LaarhovenSearch::CancelSwap(Solution& sol, unsigned parent, unsigned child)
{
    sol.SwapOperations(child, parent);
    std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
}


void LaarhovenSearch::UpdateOp(const Solution& sol, unsigned oid)
{
    // variables de l'algorithme
    unsigned date_old, date_job, date_mac, date_par;

    // stockage de l'ancienne date
    if (is_changed[oid] >= OpUpdate::Changed) {
        date_old = new_start_date[oid];
    }
    else {
        date_old = sol.startDate[oid];
    }

    // récupération des nouvelles dates parent / parent disj
    if (sol.macParent[oid] == -1) {
        date_mac = 0;
    }
    else if (is_changed[sol.macParent[oid]] >= OpUpdate::Changed) {
        date_mac = new_end_date[sol.macParent[oid]];
    }
    else {
        date_mac = sol.endDate[sol.macParent[oid]];
    }

    if (ref_pb.prevOperation[oid] == -1) {
        date_job = 0;
    }
    else if (is_changed[ref_pb.prevOperation[oid]] >= OpUpdate::Changed) {
        date_job = new_end_date[ref_pb.prevOperation[oid]];
    }
    else {
        date_job = sol.endDate[ref_pb.prevOperation[oid]];
    }

    // date critique
    if (date_job < date_mac) {
        date_par = date_mac;
        new_is_crit_mac[oid] = true;
    }
    else {
        date_par = date_job;
        new_is_crit_mac[oid] = false;
    }

    // nouvelles valeurs
    new_start_date[oid] = date_par;
    new_end_date[oid] = date_par + ref_pb.timeOnMachine[oid];
}


void LaarhovenSearch::AddSuccessors(const Solution& sol, unsigned oid)
{
    // ajout des successeurs à traiter
    if (sol.macChild[oid] != -1) {
        if (is_changed[sol.macChild[oid]] % 2 != OpUpdate::ToChange) {
            ops_to_move.push_back(sol.macChild[oid]);
            ++is_changed[sol.macChild[oid]]; // Unchanged -> ToChange and Changed -> ChangedToChange
        }
    }
    if (ref_pb.nextOperation[oid] != -1) {
        if (is_changed[ref_pb.nextOperation[oid]] % 2 != OpUpdate::ToChange) {
            ops_to_move.push_back(ref_pb.nextOperation[oid]);
            ++is_changed[ref_pb.nextOperation[oid]];
        }
    }
}