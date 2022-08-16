#ifndef RANDOM_PLACEMENT_H_
#define RANDOM_PLACEMENT_H_

#include "const-heuristic/const_heuristic.h"

class RandomPlacement : public CandidateHeuristic<CandidateJob> {
public:
	RandomPlacement(const Problem&, unsigned seed = 0);
	Solution& operator()(Solution&);
};
#endif // !RANDOM_PLACEMENT_H_
