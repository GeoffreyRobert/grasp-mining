#include <limits>
#include <chrono>

#include "solver/solver.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"
#include "miner/solution_filter.h"
#include "miner/data_miner.h"

using namespace std::chrono;

Solver::Solver(
    ConstHeuristic& init_heuristic
  , ConstHeuristic& const_heuristic
  , LocalSearch& local_search
  , SolutionFilter& solution_filter
  , DataMiner& data_miner
  , unsigned pop_size)
  : initHeuristic(init_heuristic)
  , constHeuristic(const_heuristic)
  , localSearch(local_search)
  , solutionFilter(solution_filter)
  , dataMiner(data_miner)
  , populationSize(pop_size)
  , runtime(0)
{}

Solver::Solver(Solver&& other)
  : initHeuristic(other.initHeuristic)
  , constHeuristic(other.constHeuristic)
  , localSearch(other.localSearch)
  , solutionFilter(other.solutionFilter)
  , dataMiner(other.dataMiner)
  , populationSize(other.populationSize)
  , runtime(other.runtime)
{}

Solution Solver::Solve(const Problem& problem)
{
  // solutions manipul�es dans l'algo
  vector<Solution> solution_set(populationSize, problem);

  // The first uninitialized solution from the set is the reference
  Solution* best_solution = &solution_set.front();

  // d�marre le timer
  auto init = high_resolution_clock::now();

  /*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
  for (Solution& sol : solution_set)  {
    initHeuristic(sol);

    localSearch.hit_count = 0;
    localSearch(sol);

    // Mise � jour de la meilleure solution rencontr�e
    if (sol.Makespan() < best_solution->Makespan()) {
      best_solution = &sol;
    }
  }

  auto& filtered_solutions = solutionFilter(solution_set);
  dataMiner(filtered_solutions);

  // Timer
  auto end = high_resolution_clock::now();
  runtime = duration_cast<milliseconds>(end - init);

  return std::move(*best_solution);
}
