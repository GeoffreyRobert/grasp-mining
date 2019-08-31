#ifndef SOLUTION_
#define SOLUTION_

#include <functional>
#include <vector>

#include "problem.h"
#include "solution_impl.h"

using std::vector;

struct OpUpdate {
    enum update_t {
        Unchanged = 0,
        ToChange = 1,
        Changed = 2,
        ChangedToChange = 3
    };
};

class Solution {
public:
	Solution(const Problem& problem, AccessImpl& implSol);

	Solution(const Solution& other);

	Solution& operator=(const Solution& other);

	Solution& operator=(Solution&& other);

	void AddOperation(int oid, int start, int end, int parent, bool is_on_mac);

	void SwapOperations(int parent, int child);

	void DoChanges(vector<unsigned>& new_start_date, vector<unsigned>& new_end_date,
	    vector<bool>& new_is_crit_mac, vector<OpUpdate>& is_changed);

	const Problem& problem;

	// Plannification de la solution
	unsigned int makespan = 0;
	unsigned int criticalOp;

	VecAccess<unsigned int> startDate;				// date de début de chaque opération
    VecAccess<unsigned int> endDate;				// date de fin de chaque operation

    VecAccess<int> macParent;						// parent sur la machine
    VecAccess<bool> isCritMachine;					// parent critique sur la machine
    VecAccess<int> macChild;						// successeur(s) sur la machine
};
#endif
