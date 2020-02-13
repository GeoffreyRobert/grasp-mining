#ifndef RANDOM_PLACEMENT_H_
#define RANDOM_PLACEMENT_H_

#include <random>

#include "const_heuristic.h"

class RandomPlacement : public ConstHeuristic {
public:
	RandomPlacement(const Problem&);
	Solution& operator()(Solution&);

private:
	std::mt19937 generator;
	int ChooseRandomJob(vector<int>& jobs_left);
};
#endif // !RANDOM_PLACEMENT_H_
