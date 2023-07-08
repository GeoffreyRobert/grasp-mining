#ifndef BINATO_HEURISTIC_H_
#define BINATO_HEURISTIC_H_

#include <random>
#include <vector>

#include "const-heuristic/candidate_selector.h"

class RestrictedSelector : public CandidateSelector
{
public:
  RestrictedSelector(const Problem&, double alpha = 0.5, unsigned seed = 0);
  size_t operator()(Solution&, vector<CandidateJob>& candidates) override;

private:
  double _alpha = 0.5;

  // Restricted Candidate List
  vector<size_t> rc_list;
};

#endif // BINATO_HEURISTIC_H_
