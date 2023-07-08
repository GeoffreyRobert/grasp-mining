#ifndef CANDIDATE_SELECTOR_
#define CANDIDATE_SELECTOR_

#include <random>
#include <vector>

#include "solver/solver_module.h"

using std::vector; using std::size_t;

class Solution;
class CandidateJob;

class CandidateSelector : public SolverModule
{
public:
  CandidateSelector(const Problem&, unsigned seed);
  virtual ~CandidateSelector() {};
  virtual size_t operator()(Solution&, vector<CandidateJob>& candidates) = 0;

protected:
	std::mt19937 _r_generator;
};

#endif // CANDIDATE_SELECTOR_
