#ifndef LAARHOVEN_H_
#define LAARHOVEN_H_

#include "local_search.h"
#include "data/problem.h"
#include "data/solution.h"

class LaarhovenSearch : public LocalSearch {
public:
	// Recherche locale le long du chemin critique
	Solution& operator()(const Problem& problem, Solution& solution);
};

#endif // LAARHOVEN_H_	