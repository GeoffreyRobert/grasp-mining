#ifndef SOLUTION_
#define SOLUTION_

#include <functional>
#include <vector>

#include "problem.h"

using std::vector;

enum OpUpdate {
    Unchanged = 0,
    ToChange = 1,
    Changed = 2,
    ChangedToChange = 3
};
OpUpdate& operator++(OpUpdate&);
OpUpdate& operator--(OpUpdate&);


class Solution {
public:
	Solution(const Problem& problem);

	Solution(const Solution& other);

	Solution& operator=(const Solution& other);

	Solution& operator=(Solution&& other) noexcept;

	void AddOperation(int oid, int start, int end, int parent, bool is_on_mac);

	void SwapOperations(int parent, int child);

	void DoChanges(
        unsigned new_critical, unsigned new_makespan,
        vector<OpUpdate>& is_changed, vector<unsigned>& new_start_date, 
        vector<unsigned>& new_end_date, vector<bool>& new_is_crit_mac);

	const Problem& problem;

	// Plannification de la solution
	unsigned int makespan = 0;
	unsigned int criticalOp = 0;

	vector<unsigned> startDate;				// date de début de chaque opération
    vector<unsigned> endDate;				// date de fin de chaque operation

    vector<int> macParent;						// parent sur la machine
    vector<bool> isCritMachine;					// parent critique sur la machine
    vector<int> macChild;						// successeur(s) sur la machine
};
#endif
