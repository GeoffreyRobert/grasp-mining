#ifndef CONST_HEURITIC_H_
#define CONST_HEURITIC_H_

#include "data/problem.h"
#include "data/solution.h"
#include "solver/solver_module.h"

class ConstHeuristic : public SolverModule {
public:
  using SolverModule::SolverModule;
  virtual ~ConstHeuristic() {};
	virtual Solution& operator()(Solution&) = 0;
};
#endif // CONST_HEURITIC_H_
