#include <limits>
#include <chrono>

#include "solver.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"

using std::chrono::high_resolution_clock;

Solver::Solver(ConstHeuristic& init_heuristic, ConstHeuristic& const_heuristic,
			   LocalSearch& local_search, DataMiner& data_miner) :
	constHeuristic(const_heuristic), initHeuristic(init_heuristic),
	localSearch(local_search), dataMiner(data_miner), runtime(0.)
{}

Solver::Solver(Solver&& other) :
	constHeuristic(other.constHeuristic), initHeuristic(initHeuristic),
	localSearch(other.localSearch), dataMiner(other.dataMiner)
{}

Solution Solver::Solve(const Problem& problem) {
	// solutions manipulées dans l'algo
	Solution best_solution(problem);
	vector<Solution> solution_set(populationSize, best_solution);

	// on met le cout de la meilleure solution tres haut
	best_solution.makespan = std::numeric_limits<int>::max();

	// démarre le timer
	high_resolution_clock::time_point init = high_resolution_clock::now();

	/*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
	for (Solution& sol : solution_set)  {
		initHeuristic(problem, sol);

		localSearch(problem, sol);

		// Mise à jour de la meilleure solution rencontrée
		if (sol.makespan < best_solution.makespan) {
			best_solution = sol;
		}
	}

	dataMiner(problem, solution_set);

	for (Solution& sol : solution_set) {
		constHeuristic(problem, sol);

		localSearch(problem, sol);

		// Mise à jour de la meilleure solution rencontrée
		if (sol.makespan < best_solution.makespan) {
			best_solution = sol;
		}
	}

	// Timer
	high_resolution_clock::time_point end = high_resolution_clock::now();
	runtime = (end - init);

	return best_solution;

}