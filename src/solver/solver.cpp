#include <limits>
#include <chrono>

#include "solver/solver.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"
#include "miner/data_miner.h"

using namespace std::chrono;

Solver::Solver(ConstHeuristic& init_heuristic, ConstHeuristic& const_heuristic,
			   LocalSearch& local_search, DataMiner& data_miner, unsigned pop_size) :
	initHeuristic(init_heuristic), constHeuristic(const_heuristic),
	localSearch(local_search), dataMiner(data_miner), 
	populationSize(pop_size), runtime(0)
{}

Solver::Solver(Solver&& other) :
	initHeuristic(other.initHeuristic), constHeuristic(other.constHeuristic),
	localSearch(other.localSearch), dataMiner(other.dataMiner),
  populationSize(other.populationSize), runtime(other.runtime)
{}

Solution Solver::Solve(const Problem& problem)
{
	// solutions manipulées dans l'algo
	Solution best_solution(problem);
	vector<Solution> solution_set(populationSize, best_solution);

	// on met le cout de la meilleure solution tres haut
	best_solution.makespan = std::numeric_limits<int>::max();

	// démarre le timer
	auto init = high_resolution_clock::now();

	/*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
	for (Solution& sol : solution_set)  {
		initHeuristic(sol);

		localSearch.hit_count = 0;
		localSearch(sol);

		// Mise à jour de la meilleure solution rencontrée
		if (sol.makespan < best_solution.makespan) {
			best_solution = sol;
		}
	}

	// Timer
	auto end = high_resolution_clock::now();
	runtime = duration_cast<milliseconds>(end - init);

	return best_solution;

}
