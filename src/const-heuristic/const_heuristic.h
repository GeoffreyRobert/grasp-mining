#ifndef CONST_HEURITIC_H_
#define CONST_HEURITIC_H_

#include "data/problem.h"
#include "data/solution.h"

class ConstHeuristic {
public:
	virtual Solution& operator()(const Problem& problem, Solution& solution) = 0;

};
#endif // CONST_HEURITIC_H_
