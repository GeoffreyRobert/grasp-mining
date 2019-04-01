#include <algorithm>
#include <utility>
#include <deque>

#include "solution.h"

using std::deque; using std::map;

Solution::Solution(const Problem& problem) :
	problem(problem), startDate(problem.size),
	endDate(problem.size), macParent(problem.size),
	macChild(problem.size), isCritMachine(problem.size)
{
};

Solution::Solution(const Solution& other) :
	problem(other.problem) {
	makespan = other.makespan;
	criticalOp = other.criticalOp;
	startDate = other.startDate;
	endDate = other.endDate;
	macParent = other.macParent;
	macChild = other.macChild;
	isCritMachine = other.isCritMachine;
}

// Move assignment operator.
Solution& Solution::operator=(Solution&& other) {
	if (this != &other) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

		macParent = std::move(other.macParent);
		startDate = std::move(other.startDate);
		endDate = std::move(other.endDate);

		macParent = std::move(other.macParent);
		macChild = std::move(other.macChild);
		isCritMachine = std::move(other.isCritMachine);
	}
	return *this;

}

// Copy assignment operator.
Solution& Solution::operator=(const Solution& other) {
	if (this != &other) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

		macParent = other.macParent;
		startDate = other.startDate;
		endDate = other.endDate;

		macParent = other.macParent;
		macChild = other.macChild;
		isCritMachine = other.isCritMachine;
	}
	return *this;

}


void Solution::AddOperation(int oid, int start, int end, int parent, bool is_on_mac) {
	startDate[oid] = start;
	endDate[oid] = end;
	macParent[oid] = parent;
	isCritMachine[oid] = is_on_mac;
	if (parent != -1)
		macChild[parent] = oid;
	if (endDate[oid] > makespan) {
		criticalOp = oid;
		makespan = endDate[oid];
	}
}


bool Solution::CheckAndSwap(int parent, int child) {
	if (problem.jobForOp[parent] == problem.jobForOp[child]) {
		return false;
	}

	// cas qui allonge forcément le chemin critique
	if (problem.prevOperation[child] != -1 && 
		startDate[parent] < endDate[problem.prevOperation[child]]) {
		return false;
	}

	// variables de l'algorithme
	int oid, date_job, date_mac, date_par, date_old;
	bool is_on_mac;
	unsigned tmp_makespan = makespan, tmp_critical = criticalOp;

	// stockage des operations à déplacer
	deque<int> ops_to_move;

	// données temporaires
	map<int, int> new_start_date;
	map<int, int> new_end_date;
	map<int, bool> new_is_on_mac;

	// initialisation des successeurs à traiter
	if (problem.nextOperation[parent] != -1)
		ops_to_move.push_back(problem.nextOperation[parent]);
	if (macChild[child] != -1)
		ops_to_move.push_back(macChild[child]);
	if (problem.nextOperation[child] != -1)
		ops_to_move.push_back(problem.nextOperation[child]);

	// intialisation des dates
	if (problem.prevOperation[child] != -1) {
		date_job = endDate[problem.prevOperation[child]];
	}
	else {
		date_job = 0;
	}
	if (macParent[parent] != -1) {
		date_mac = endDate[macParent[parent]];
	}
	else {
		date_mac = 0;
	}
	if (date_job < date_mac) {
		date_par = date_mac;
		new_is_on_mac.insert(std::make_pair(child, true));
	}
	else {
		date_par = date_job;
		new_is_on_mac.insert(std::make_pair(child, false));
	}
	new_start_date.insert(std::make_pair(child, date_par));
	new_end_date.insert(
		std::make_pair(child, new_start_date[child] + problem.timeOnMachine[child]));

	new_start_date.insert(std::make_pair(parent, new_end_date[child]));
	new_end_date.insert(
		std::make_pair(parent, new_start_date[parent] + problem.timeOnMachine[parent]));
	new_is_on_mac.insert(std::make_pair(parent, true));

	while (!ops_to_move.empty()) {
		// récupération des identifiants
		oid = ops_to_move.front();
		ops_to_move.pop_front();

		// récupération des dates parent / parent disj
		if (macParent[oid] == -1) {
			date_mac = 0;
		}
		else if (new_start_date.count(macParent[oid]) == 1) {
			date_mac = new_end_date[macParent[oid]];
		}
		else {
			date_mac = endDate[macParent[oid]];
		}

		if (problem.prevOperation[oid]) {
			date_job = 0;
		}
		else if (new_start_date.count(problem.prevOperation[oid]) == 1) {
			date_job = new_end_date[problem.prevOperation[oid]];
		}
		else {
			date_job = endDate[problem.prevOperation[oid]];
		}

		// date critique
		if (date_job < date_mac) {
			is_on_mac = true;
			date_par = date_mac;
		}
		else {
			is_on_mac = false;
			date_par = date_job;
		}

		// ancienne date de démarrage de l'opération
		if (new_start_date.count(oid) == 1) {
			date_old = new_start_date[oid];
		}
		else {
			date_old = startDate[oid];
		}

		// nouvelles valeurs
		new_start_date.insert(std::make_pair(oid, date_par));
		new_end_date.insert(std::make_pair(oid, date_par + problem.timeOnMachine[oid]));
		new_is_on_mac.insert(std::make_pair(oid, is_on_mac));

		// aucun changement
		if (date_par == date_old) {
			return false;
		}

		// ajout des successeurs à traiter
		if (macChild[oid] != -1 && 
			std::find(ops_to_move.begin(), ops_to_move.end(), macChild[oid]) ==
			ops_to_move.end()) {
			ops_to_move.push_back(macChild[oid]);
		}
		if (problem.nextOperation[oid] != -1) {
			if (std::find(ops_to_move.begin(), ops_to_move.end(), problem.nextOperation[oid]) ==
				ops_to_move.end()) {
				ops_to_move.push_back(problem.nextOperation[oid]);
			}
		}

		// vérification de l'operation critique
		if (problem.nextOperation[oid] == -1 && macChild[oid] == -1) {
			if (oid != tmp_critical) {
				// mise à jour
				if (tmp_makespan < new_end_date[oid]) {
					tmp_critical = oid;
					tmp_makespan = new_end_date[oid];
				}
			}
			else if (date_par < date_old) {
				for (int jid = 0, last_op; jid < problem.nJob; ++jid) {
					if (jid != problem.jobForOp[oid]) {
						last_op = problem.operationNumber[jid].back();
						if (new_start_date.count(last_op) == 1) {
							date_job = new_end_date[last_op];
						}
						else {
							date_job = endDate[last_op];
						}
						if (date_job > tmp_makespan) {
							tmp_critical = last_op;
							tmp_makespan = date_job;
						}
					}
				}
			}
			else {
				tmp_makespan = new_end_date[oid];
			}
		}
	}

	if (tmp_makespan < makespan) {
		criticalOp = tmp_critical;
		makespan = tmp_makespan;
		DoChanges(parent, child, new_start_date, new_end_date, new_is_on_mac);
		return true;
	}
	else {
		return false;
	}
}

void Solution::DoChanges(int parent, int child, map<int, int> new_start_date,
	map<int, int> new_end_date, map<int, bool> new_is_on_mac) {
	// inversion des deux operations
	macParent[child] = macParent[parent];
	macChild[macParent[parent]] = child;

	macChild[parent] = macChild[child];
	macParent[macChild[child]] = parent;

	macParent[parent] = child;
	macChild[child] = parent;

	int oid;
	// mise a jour de la solution
	for (std::map<int, int>::iterator iter = new_start_date.begin(); 
		iter != new_start_date.end(); ++iter) {
		oid = iter->first;
		startDate[oid] = iter->second;
		endDate[oid] = new_end_date[oid];
		isCritMachine[oid] = new_is_on_mac[oid];
	}
}