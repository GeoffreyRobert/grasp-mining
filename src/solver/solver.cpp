#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
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
  , DataMiner& data_miner
  , ConstHeuristic& hybrid_heuristic
  , unsigned pop_size)
  : initHeuristic(init_heuristic)
  , localSearch(local_search)
  , dataMiner(data_miner)
  , hybridHeuristic(hybrid_heuristic)
  , populationSize(pop_size)
  , runtime(0)
{}

Solver::Solver(Solver&& other)
  : initHeuristic(other.initHeuristic)
  , localSearch(other.localSearch)
  , dataMiner(other.dataMiner)
  , hybridHeuristic(other.hybridHeuristic)
  , populationSize(other.populationSize)
  , runtime(other.runtime)
{}

Solution Solver::Solve(const Problem& problem)
{
  // solutions manipulées dans l'algo
  unsigned init_population = populationSize / 2;
  vector<Solution> solution_set(init_population, problem);

  // démarre le timer
  auto init = high_resolution_clock::now();

  /*while (populationSize++< populationSize && best_solution.makespan > problem.lowerBound && time cond) {*/
  for (Solution& sol : solution_set)
  {
    initHeuristic(sol);
    localSearch(sol);
  }
  double init_makespan_mean = std::accumulate(
      begin(solution_set), end(solution_set), 0.0
      , [](double acc, const auto& sol) { return acc + sol.Makespan(); });
  init_makespan_mean /= static_cast<double>(solution_set.size());
  std::cout << "init_makespan_mean=" << init_makespan_mean << '\n';

  Solution best_solution =
      *std::min_element(std::begin(solution_set), std::end(solution_set));
  // data miner is implicitly linked to the hybrid heuristic
  dataMiner(solution_set);

  solution_set.clear();
  unsigned hybrid_population = populationSize - init_population;
  solution_set.resize(hybrid_population, problem);
  for (Solution& sol : solution_set)
  {
    hybridHeuristic(sol);
    localSearch(sol);
  }
  double hybrid_makespan_mean = std::accumulate(
      begin(solution_set), end(solution_set), 0.0
      , [](double acc, const auto& sol) { return acc + sol.Makespan(); });
  hybrid_makespan_mean /= static_cast<double>(solution_set.size());
  std::cout << "hybrid_makespan_mean=" << hybrid_makespan_mean << '\n';
  std::cout << "makespan_mean_improv=" << init_makespan_mean - hybrid_makespan_mean << '\n';

  auto best_iter = std::min_element(std::begin(solution_set), std::end(solution_set));
  if (*best_iter < best_solution)
    best_solution = std::move(*best_iter);

  // Timer
  auto end = high_resolution_clock::now();
  runtime = duration_cast<milliseconds>(end - init);

  return best_solution;
}
