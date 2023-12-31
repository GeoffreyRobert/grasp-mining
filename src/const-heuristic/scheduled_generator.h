#ifndef GREEDY_GENERATOR_
#define GREEDY_GENERATOR_

#include "const-heuristic/candidate_generator.h"

class ScheduledGenerator : public CandidateGenerator
{
public:
  using CandidateGenerator::CandidateGenerator;
  const vector<CandidateJob>& operator()(Solution&) override;
};

#endif // GREEDY_GENERATOR_
