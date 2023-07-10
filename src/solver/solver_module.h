#ifndef SOLVER_MODULE_H_
#define SOLVER_MODULE_H_

#include <chrono>

class Problem;

using namespace std::chrono;

class SolverModule {
public:
	SolverModule(const Problem& problem) : ref_pb(problem) {};

protected:
  const Problem& ref_pb;
  std::chrono::microseconds runtime{0};
};

#endif // SOLVER_MODULE_H_
