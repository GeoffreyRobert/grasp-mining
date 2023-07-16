#ifndef CANDIDATE_GENERATOR_
#define CANDIDATE_GENERATOR_

#include <vector>
#include <ostream>

#include "solver/solver_module.h"

using std::vector; using std::size_t;

class Solution;
class CandidateJob;

class CandidateGenerator : public SolverModule
{
public:
  CandidateGenerator(const Problem&);
  virtual ~CandidateGenerator() {};
  virtual void Init();
  virtual const vector<CandidateJob>& operator()(Solution&);
  virtual void IncrementJob(size_t job_idx);

  friend std::ostream& operator<<(std::ostream&, const CandidateGenerator&);

protected:
  virtual void Write(std::ostream&) const;
  vector<CandidateJob> _candidate_jobs;  // jobs left to be added to the solution
};

#endif // CANDIDATE_GENERATOR_
