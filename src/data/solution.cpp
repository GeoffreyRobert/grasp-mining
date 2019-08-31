#include <cassert>

#include "solution.h"
#include "solution_impl.h"

using std::vector;

Solution::Solution(const Problem& problem) :
	problem(problem), implSol(AccessImpl(problem)),
    startDate(&AccessImpl::getStartDate), endDate(&AccessImpl::getEndDate),
    macParent(&AccessImpl::getMacParent), macChild(&AccessImpl::getMacChild),
    isCritMachine(&AccessImpl::getIsCritMachine)
{
}

Solution::Solution(const Solution& other) :
	problem(other.problem), implSol(other.implSol)
{
}

// Move assignment operator.
Solution& Solution::operator=(Solution&& other) {
	if (this != &other and *problem == *other.problem) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

		implSol = std::move(other.implSol);
        return *this;
    }
    else {
        return nullptr;
    }
}

// Copy assignment operator.
Solution& Solution::operator=(const Solution& other) {
	if (this != &other and *problem == *other.problem) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

		implSol = other.implSol;
        return *this;
    }
    else {
        return nullptr;
    }
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

void Solution::DoChanges(vector<unsigned>& new_start_date, vector<unsigned>& new_end_date,
						 vector<bool>& new_is_crit_mac, vector<OpUpdate>& is_changed) {
	for (int oid = 0; oid < problem.size; ++oid) {
		if (is_changed[oid] == OpUpdate::Changed) {
			startDate[oid] = new_start_date[oid];
			endDate[oid] = new_end_date[oid];
			isCritMachine[oid] = new_is_crit_mac[oid];
		}
        // throw une exception ici s'il reste des opérations non traitées ?
	}
}

void Solution::SwapOperations(int parent, int child) {
	// inversion des deux operations
	macParent[child] = macParent[parent];
	if (macParent[parent] != -1)
		macChild[macParent[parent]] = child;

	macChild[parent] = macChild[child];
	if (macChild[child] != -1)
		macParent[macChild[child]] = parent;

	macParent[parent] = child;
	macChild[child] = parent;
}