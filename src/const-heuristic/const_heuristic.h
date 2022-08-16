#ifndef CONST_HEURITIC_H_
#define CONST_HEURITIC_H_

#include <random>

#include "data/typedefs.h"
#include "solver/solver_module.h"

using std::vector;

class Problem;
class Solution;

class ConstHeuristic : public SolverModule {
public:
  using SolverModule::SolverModule;
  virtual ~ConstHeuristic() {};
	virtual Solution& operator()(Solution&) = 0;
};

struct CandidateJob {
  JobId jid;
  OperationRank rank;
};

template<class ConstData>
class CandidateHeuristic : public ConstHeuristic {
public:
  CandidateHeuristic(const Problem& problem, unsigned seed);
  virtual ~CandidateHeuristic() {};
	virtual Solution& operator()(Solution&) = 0;

protected:
  virtual void Init();

	std::mt19937 generator;
  vector<ConstData> candidate_jobs;  // jobs left to be added to the solution
};

#endif // CONST_HEURITIC_H_
