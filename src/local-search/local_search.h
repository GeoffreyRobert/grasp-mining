#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

#include "data/problem.h"
#include "data/solution.h"
#include "solver/solver_module.h"

class LocalSearch : public SolverModule {
public:
	virtual Solution& operator()(const Problem& problem, Solution& solution) = 0;

	int hit_count;
};

#endif // LOCAL_SEARCH_H_

