#ifndef RANDOM_PLACEMENT_H_
#define RANDOM_PLACEMENT_H_

#include "const-heuristic/const_heuristic.h"

class RandomPlacement : public CandidateHeuristic<CandidateJob> {
public:
	RandomPlacement(
      const Problem&, CandidateGenerator<CandidateJob>& generator, unsigned seed = 0);

private:
  CandidateJob& CandidateSelection(vector<CandidateJob>&, Solution&) override;
};
#endif // !RANDOM_PLACEMENT_H_
