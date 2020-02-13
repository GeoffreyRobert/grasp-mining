#include <limits>
#include <chrono>
#include <iostream>

#include "solver.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"

using std::chrono::high_resolution_clock; using std::cout;

Solver::Solver(ConstHeuristic& init_heuristic, ConstHeuristic& const_heuristic,
			   LocalSearch& local_search, DataMiner& data_miner, int pop_size) :
	constHeuristic(const_heuristic), initHeuristic(init_heuristic),
	localSearch(local_search), dataMiner(data_miner), runtime(0.), 
	populationSize(pop_size)
{}

Solver::Solver(Solver&& other) :
	constHeuristic(other.constHeuristic), initHeuristic(other.initHeuristic),
	localSearch(other.localSearch), dataMiner(other.dataMiner),
    runtime(other.runtime), populationSize(other.populationSize)
{}

Solution Solver::Solve(const Problem& problem)
{
	// solutions manipulées dans l'algo
	Solution best_solution(problem);
	vector<Solution> solution_set(populationSize, best_solution);

	// on met le cout de la meilleure solution tres haut
	best_solution.makespan = std::numeric_limits<int>::max();

	// démarre le timer
	high_resolution_clock::time_point init = high_resolution_clock::now();

	/*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
	for (Solution& sol : solution_set)  {
		initHeuristic(sol);

		localSearch.hit_count = 0;
		localSearch(sol);
		cout << "local search improvement: " << localSearch.hit_count << std::endl;

		// Mise à jour de la meilleure solution rencontrée
		if (sol.makespan < best_solution.makespan) {
			best_solution = sol;
		}
	}

	dataMiner(solution_set);

	for (Solution& sol : solution_set) {
		constHeuristic(sol);

		localSearch.hit_count = 0;
		localSearch(sol);
		cout << "local search improvement: " << localSearch.hit_count << std::endl;

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