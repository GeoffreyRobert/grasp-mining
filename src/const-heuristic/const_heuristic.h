#ifndef CONST_HEURISTIC_H_
#define CONST_HEURISTIC_H_

#include <vector>

#include "data/typedefs.h"
#include "solver/solver_module.h"

using std::vector;

class Problem;
class Solution;
class CandidateGenerator;
class CandidateSelector;

struct CandidateJob {
  JobId jid;
  OperationRank rank;
  int makespan;
};

class ConstHeuristic : public SolverModule {
public:
  ConstHeuristic(
      const Problem&
    , CandidateGenerator&
    , CandidateSelector&);
	Solution& operator()(Solution&);

private:
  CandidateGenerator& _generator;
  CandidateSelector& _selector;
};

#endif // CONST_HEURISTIC_H_
