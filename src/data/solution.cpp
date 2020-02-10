#include <cassert>

#include "solution.h"

using std::vector;

OpUpdate& operator++(OpUpdate& up) {
    return up = static_cast<OpUpdate>(static_cast<int>(up) + 1);
}
OpUpdate& operator--(OpUpdate& up) {
    return up = static_cast<OpUpdate>(static_cast<int>(up) - 1);
}

Solution::Solution(const Problem& problem) :
	problem(problem), startDate(problem.size), endDate(problem.size),
    macParent(problem.size), macChild(problem.size), isCritMachine(problem.size)
{
}

Solution::Solution(const Solution& other) :
	problem(other.problem), startDate(other.startDate), endDate(other.endDate),
    macParent(other.macParent), macChild(other.macChild),
    isCritMachine(other.isCritMachine)
{
}

// Move assignment operator.
Solution& Solution::operator=(Solution&& other) noexcept {
	if (this != &other && &problem == &other.problem) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

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
	if (this != &other && &problem == &other.problem) {
		makespan = other.makespan;
		criticalOp = other.criticalOp;

        startDate = other.startDate;
        endDate = other.endDate;
        macParent = other.macParent;
        macChild = other.macChild;
        isCritMachine = other.isCritMachine;
    }
    return *this;
}


void Solution::AddOperation(
        int oid, int start, int end, int parent, bool is_on_mac) {
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

void Solution::DoChanges(
        unsigned new_critical, unsigned new_makespan,
        vector<OpUpdate>& is_changed, vector<unsigned>& new_start_date,
        vector<unsigned>& new_end_date, vector<bool>& new_is_crit_mac) {
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