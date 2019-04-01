#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

#include "data/problem.h"
#include "data/solution.h"

class LocalSearch {
public:
	virtual Solution& operator()(const Problem& problem, Solution& solution) = 0;
};

#endif // LOCAL_SEARCH_H_

