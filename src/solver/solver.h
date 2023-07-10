#ifndef SOLVER_H_
#define SOLVER_H_

#include <chrono>
#include <memory>
#include <ratio>

class Problem;
class Solution;
class ConstHeuristic;
class LocalSearch;
class SolutionFilter;
class DataMiner;

class Solver {
public:
  Solver(
      ConstHeuristic& init_heuristic
    , LocalSearch&
    , DataMiner&
    , ConstHeuristic& hybrid_heuristic
    , unsigned pop_size);

  Solver(Solver&&);

  // Resout une instance de JSSP et retourne une solution
  Solution Solve(const Problem&);

  // Idem que Solve mais utilise une limite temporelle
  // void SolveWithTimeLimit(const Problem&, TimeLimit&);

  // The hybrid solver.
  ConstHeuristic& initHeuristic;
  LocalSearch& localSearch;
  DataMiner& dataMiner;
  ConstHeuristic& hybridHeuristic;

  // Size of the population of one batch
  unsigned populationSize = 500;

  // Timer
  std::chrono::milliseconds runtime;
};

#endif  // _SOLVER_H_
