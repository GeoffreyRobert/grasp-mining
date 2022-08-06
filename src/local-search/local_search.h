#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

#include "solver/solver_module.h"

class Solution;

class LocalSearch : public SolverModule {
public:
  using SolverModule::SolverModule;
  virtual ~LocalSearch() {};
  virtual Solution& operator()(Solution& solution) = 0;

  // TODO: cacher avec preproc. pour debug
  unsigned hit_count = 0;
};

#endif // LOCAL_SEARCH_H_

