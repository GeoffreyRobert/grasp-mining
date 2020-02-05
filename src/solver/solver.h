#ifndef SOLVER_H_
#define SOLVER_H_

#include <chrono>
#include <memory>
#include <ratio>

#include "const-heuristic/const_heuristic.h"
#include "data/problem.h"
#include "local-search/local_search.h"
#include "miner/data_miner.h"

class Solver {
public:
	Solver(ConstHeuristic&, ConstHeuristic&, LocalSearch&, DataMiner&, int pop_size);

	Solver(Solver&&);

	// Allocation des ressources pour les modules
	void ModulesResourcesAlloc(const Problem&);

	// Resout une instance de JSSP et retourne une solution
	Solution Solve(const Problem&);

	// Idem que Solve mais utilise une limite temporelle
	// void SolveWithTimeLimit(const Problem&, TimeLimit&);

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
