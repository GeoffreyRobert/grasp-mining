#ifndef RANDOM_SELECTOR_
#define RANDOM_SELECTOR_

#include "const-heuristic/candidate_selector.h"

class RandomSelector : public CandidateSelector
{
public:
  using CandidateSelector::CandidateSelector;
  size_t operator()(const vector<CandidateJob>& candidates) override;
};
#endif // RANDOM_SELECTOR_
