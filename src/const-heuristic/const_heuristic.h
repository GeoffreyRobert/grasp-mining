#ifndef CONST_HEURISTIC_H_
#define CONST_HEURISTIC_H_

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
	Solution& operator()(Solution&);

protected:
  virtual std::vector<ConstData>& CandidatesInitialization();
  virtual ConstData& CandidateSelection(vector<ConstData>&, Solution&) = 0;

	std::mt19937 generator;

private:
  vector<ConstData> _candidate_jobs;  // jobs left to be added to the solution
};

#endif // CONST_HEURISTIC_H_
