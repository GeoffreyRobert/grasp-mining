#ifndef SOLVER_H_
#define SOLVER_H_

#include <chrono>
#include <memory>
#include <ratio>

class Problem;
class Solution;
class ConstHeuristic;
class LocalSearch;
class DataMiner;

class Solver {
public:
	Solver(ConstHeuristic&, ConstHeuristic&, LocalSearch&, DataMiner&, unsigned pop_size);

	Solver(Solver&&);

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
	unsigned populationSize = 500;

	// Timer
	std::chrono::duration<double, std::micro> runtime;
};

#endif  // _SOLVER_H_
