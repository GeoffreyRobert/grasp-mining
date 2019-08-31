#ifndef LAARHOVEN_H_
#define LAARHOVEN_H_

#include <deque>

#include "local_search.h"
#include "data/problem.h"
#include "data/solution.h"

using std::deque; using std::vector;

class LaarhovenSearch : public LocalSearch {
public:
	void ResourcesAlloc(const Problem&);

	// Recherche locale le long du chemin critique
	Solution& operator()(const Problem& problem, Solution& solution);

	int hit_count = 0;

private:
    bool CheckAndSwap(const Problem&, Solution&, int parent, int child);

	// stockage des operations � d�placer
	deque<int> ops_to_move;

	// stockage des modifications de la solution
	unsigned tmp_critical, tmp_makespan;
	vector<OpUpdate> is_changed;
	vector<unsigned> new_start_date;
	vector<unsigned> new_end_date;
	vector<bool> new_is_crit_mac;
};

#endif // LAARHOVEN_H_	