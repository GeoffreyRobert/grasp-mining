#ifndef CANDIDATE_GENERATOR_
#define CANDIDATE_GENERATOR_

#include <vector>

#include "solver/solver_module.h"

using std::vector;

class CandidateJob;

class CandidateGenerator : public SolverModule
{
public:
  using SolverModule::SolverModule;
  vector<CandidateJob>& Init();
  void IncrementJob(CandidateJob& job);

private:
  vector<CandidateJob> _candidate_jobs;  // jobs left to be added to the solution
};

#endif // CANDIDATE_GENERATOR_
