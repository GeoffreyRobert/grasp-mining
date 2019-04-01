#ifndef SOLVER_H_
#define SOLVER_H_

#include <memory>

#include "const-heuristic/const_heuristic.h"
#include "data/problem.h"
#include "local-search/local_search.h"
#include "miner/data_miner.h"

class Solver {
public:
	Solver(ConstHeuristic&, ConstHeuristic&, LocalSearch&, DataMiner&);

	Solver(Solver&&);

	// Solves the given JSSP and returns the solve status. See the ProblemStatus 
	// documentation for a description of the different values.
	Solution Solve(const Problem&);

	// Same as Solve() but use the given time limit.
	//void SolveWithTimeLimit(const Problem&, TimeLimit&);

	// The hybrid solver.
	ConstHeuristic& initHeuristic;
	ConstHeuristic& constHeuristic;
	LocalSearch& localSearch;
	DataMiner& dataMiner;

	// Size of the population of one batch
	int populationSize = 500;

	// Timer
	std::chrono::duration<double, std::micro> runtime;
};

#endif  // _SOLVER_H_
