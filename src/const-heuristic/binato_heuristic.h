#ifndef BINATO_HEURISTIC_H_
#define BINATO_HEURISTIC_H_

#include <random>
#include <vector>

#include "const_heuristic.h"

class BinatoHeuristic : public ConstHeuristic {
public:
	BinatoHeuristic();
	BinatoHeuristic(double alpha);
	void ResourcesAlloc(const Problem&);

	// construction greedy randomisée
	Solution& operator()(const Problem&, Solution&);

private:
	double alpha = 0.5;
	std::mt19937 generator;
	int ChooseRandomJob();
};
#endif // !BINATO_HEURISTIC_H_
