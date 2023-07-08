#include <limits>
#include <chrono>

#include "data/solution.h"
#include "solver/solver.h"
#include "const-heuristic/const_heuristic.h"
#include "local-search/local_search.h"
#include "miner/solution_filter.h"
#include "miner/data_miner.h"

using namespace std::chrono;

Solver::Solver(
    ConstHeuristic& init_heuristic
  , LocalSearch& local_search
  , SolutionFilter& solution_filter
  , DataMiner& data_miner
  , unsigned pop_size)
  : initHeuristic(init_heuristic)
  , localSearch(local_search)
  , solutionFilter(solution_filter)
  , dataMiner(data_miner)
  , populationSize(pop_size)
  , runtime(0)
{}

Solver::Solver(Solver&& other)
  : initHeuristic(other.initHeuristic)
  , localSearch(other.localSearch)
  , solutionFilter(other.solutionFilter)
  , dataMiner(other.dataMiner)
  , populationSize(other.populationSize)
  , runtime(other.runtime)
{}

Solution Solver::Solve(const Problem& problem)
{
  // solutions manipulées dans l'algo
  vector<Solution> solution_set(populationSize, problem);

  // démarre le timer
  auto init = high_resolution_clock::now();

  /*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
  for (Solution& sol : solution_set)  {
    initHeuristic(sol);

    localSearch.hit_count = 0;
    localSearch(sol);
  }

  auto& filtered_solutions = solutionFilter(solution_set);
  dataMiner(filtered_solutions);

  // Timer
  auto end = high_resolution_clock::now();
  runtime = duration_cast<milliseconds>(end - init);

  return std::move(filtered_solutions.front());
}
