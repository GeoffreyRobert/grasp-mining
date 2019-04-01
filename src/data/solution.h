#ifndef SOLUTION_
#define SOLUTION_

#include <map>
#include <vector>

#include "bierwirth_sequence.h"
#include "problem.h"

using std::vector; using std::map;

class Solution {
public:
	Solution(const Problem& problem);

	Solution(const Solution& other);

	Solution& operator=(const Solution& other);

	Solution& operator=(Solution&& other);

	void AddOperation(int oid, int start, int end, int parent, bool is_on_mac);

	bool CheckAndSwap(int parent, int child);

	void DoChanges(int parent, int child, map<int, int> new_start_date,
		map<int, int> new_end_date, map<int, bool> new_is_on_mac);

	const Problem& problem;

	// Plannification de la solution
	unsigned int makespan = 0;
	unsigned int criticalOp;

	vector<unsigned int> startDate;				// date de début de chaque opération
	vector<unsigned int> endDate;				// date de fin de chaque operation

	vector<int> macParent;						// parent sur la machine
	vector<bool> isCritMachine;					// parent critique sur la machine
	vector<int> macChild;						// successeur(s) sur la machine
};
#endif
