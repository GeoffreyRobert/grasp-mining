#ifndef CANDIDATE_GENERATOR_
#define CANDIDATE_GENERATOR_

#include <vector>

#include "solver/solver_module.h"

using std::vector;

template<class ConstData>
class CandidateGenerator : public SolverModule
{
public:
  CandidateGenerator(const Problem&);
  vector<ConstData>& Init();

private:
  vector<ConstData> _candidate_jobs;  // jobs left to be added to the solution
};

#endif // CANDIDATE_GENERATOR_
