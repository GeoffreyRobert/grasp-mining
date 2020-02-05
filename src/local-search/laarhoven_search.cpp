#include <utility>
#include <deque>

#include "laarhoven_search.h"

using std::deque;

void LaarhovenSearch::ResourcesAlloc(const Problem& pb) {
	is_changed.resize(pb.size, OpUpdate::Unchanged);
	new_start_date.resize(pb.size);
	new_end_date.resize(pb.size);
	new_is_crit_mac.resize(pb.size);
    ops_to_move.set_capacity(pb.size);
}

Solution& LaarhovenSearch::operator()(const Problem& pb, Solution& sol) {
	int operation = sol.criticalOp;			// op. considérée pour relocation
	int parent = sol.macParent[operation];	// parent de l'operation

	// remonter le chemin critique
	do {
		if (!sol.isCritMachine[operation]) {
            // on ignore deux ops. dans le meme job
			operation = pb.prevOperation[operation];
		}
		else if (CheckAndSwap(pb, sol, parent, operation)) {
            // on inverse deux ops. sur le chemin critique si réduction du makespan
			sol.criticalOp = tmp_critical;
			sol.makespan = tmp_makespan;
			sol.DoChanges(new_start_date, new_end_date, new_is_crit_mac, is_changed);
            std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
			hit_count++;
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


bool LaarhovenSearch::CheckAndSwap(
        const Problem& pb, Solution& sol, int parent, int child) {
    // cas qui allonge forcément le chemin critique
	if (pb.prevOperation[child] != -1 &&
		sol.startDate[parent] < sol.endDate[pb.prevOperation[child]]) {
		return false;
	}

	// variables de l'algorithme
	unsigned date_job, date_mac, date_par;

	// initialisation des successeurs à traiter
	if (pb.nextOperation[parent] != -1) {
		ops_to_move.push_back(pb.nextOperation[parent]);
		is_changed[pb.nextOperation[parent]] = OpUpdate::ToChange;
	}
	if (sol.macChild[child] != -1) { // nécessairement child sur le chemin critique
		ops_to_move.push_back(sol.macChild[child]);
		is_changed[sol.macChild[child]] = OpUpdate::ToChange;
	}
	if (pb.nextOperation[child] != -1) {
		ops_to_move.push_back(pb.nextOperation[child]);
		is_changed[pb.nextOperation[child]] = OpUpdate::ToChange;
	}

	// intialisation des dates pour le successeur
	if (pb.prevOperation[child] != -1) {
		date_job = sol.endDate[pb.prevOperation[child]];
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
	new_end_date[child] = date_par + pb.timeOnMachine[child];

	// intialisation des dates pour le parent
	if (pb.prevOperation[parent] != -1) {
		date_job = sol.endDate[pb.prevOperation[parent]];
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
	new_end_date[parent] = date_par + pb.timeOnMachine[parent];

	// inversion parent / successeur dans l'ordre sur les machines
	sol.SwapOperations(parent, child);

	// parent et successeur initiaux sont modifiés
	is_changed[parent] = OpUpdate::Changed;
	is_changed[child] = OpUpdate::Changed;

    unsigned oid;
    unsigned date_old;
    // stack vs queue en termes de perfs ? stack: localité, queue: moins de doubles accès
	while (!ops_to_move.empty()) {
		// récupération des identifiants
		oid = ops_to_move.front();
		ops_to_move.pop_front();

		// récupération des dates parent / parent disj
		if (sol.macParent[oid] == -1) {
			date_mac = 0;
		}
		else if (is_changed[sol.macParent[oid]] >= OpUpdate::Changed) {
			date_mac = new_end_date[sol.macParent[oid]];
		}
		else {
			date_mac = sol.endDate[sol.macParent[oid]];
		}

		if (pb.prevOperation[oid] == -1) {
			date_job = 0;
		}
		else if (is_changed[pb.prevOperation[oid]] >= OpUpdate::Changed) {
			date_job = new_end_date[pb.prevOperation[oid]];
		}
		else {
			date_job = sol.endDate[pb.prevOperation[oid]];
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

		// stockage de l'ancienne date
		if (is_changed[oid] >= OpUpdate::Changed) {
			date_old = new_start_date[oid];
		}
		else {
			date_old = sol.startDate[oid];
		}

		// aucun changement par rapport à l'ancienne date
		if (date_par == date_old) {
            if (oid == sol.criticalOp) { // au mieux aussi mauvais que la solution actuelle
                sol.SwapOperations(child, parent);
                std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
                return false;
            }
            ++is_changed[oid]; // ToChange -> Unchanged and ChangedToChange -> Changed
			continue;
		}

		// nouvelles valeurs
		new_start_date[oid] = date_par;
		new_end_date[oid] = date_par + pb.timeOnMachine[oid];

        // vérification que la nouvelle solution reste sub-critique
        if (pb.nextOperation[oid] == -1 && sol.macChild[oid] == -1 && new_end_date[oid] >= sol.makespan) {
            sol.SwapOperations(child, parent);
            std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);

            return false;
        }

		// ajout des successeurs à traiter
        is_changed[oid] = OpUpdate::Changed;
		if (sol.macChild[oid] != -1) {
			if (is_changed[sol.macChild[oid]] % 2 != OpUpdate::ToChange) {
				ops_to_move.push_back(sol.macChild[oid]);
                ++is_changed[sol.macChild[oid]]; // Unchanged -> ToChange and Changed -> ChangedToChange
			}
		}
		if (pb.nextOperation[oid] != -1) {
			if (is_changed[pb.nextOperation[oid]] % 2 != OpUpdate::ToChange) {
				ops_to_move.push_back(pb.nextOperation[oid]);
                ++is_changed[pb.nextOperation[oid]];
			}
		}
	}

    tmp_makespan = 0, tmp_critical = -1;
    for (int jid = 0, last_op; jid < pb.nJob; ++jid) {
        last_op = pb.operationNumber[jid].back();
        if (is_changed[last_op] >= OpUpdate::Changed) {
            date_job = new_end_date[last_op];
        }
        else {
            date_job = sol.endDate[last_op];
        }
        if (date_job > tmp_makespan) {
            tmp_critical = last_op;
            tmp_makespan = date_job;
        }
    }

	if (tmp_makespan < sol.makespan) {
		return true;
	}
	else {
        sol.SwapOperations(child, parent);
        std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
        return false;
	}
}