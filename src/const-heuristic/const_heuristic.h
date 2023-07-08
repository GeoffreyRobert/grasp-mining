#ifndef CONST_HEURISTIC_H_
#define CONST_HEURISTIC_H_

#include <random>

#include "data/typedefs.h"
#include "solver/solver_module.h"

using std::vector;

class Problem;
class Solution;
template<class ConstData> class CandidateGenerator;

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
  CandidateHeuristic(
      const Problem& problem, CandidateGenerator<ConstData>& generator, unsigned seed);
  virtual ~CandidateHeuristic() {};
	Solution& operator()(Solution&);

protected:
  virtual ConstData& CandidateSelection(vector<ConstData>&, Solution&) = 0;

	std::mt19937 generator;

private:
  CandidateGenerator<ConstData>& _c_generator;
};

#endif // CONST_HEURISTIC_H_
