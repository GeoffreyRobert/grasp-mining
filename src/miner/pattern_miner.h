#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include <list>
#include <utility>

#include "data_miner.h"
#include "data/problem.h"

using std::list; using std::pair;

class PatternMiner : public DataMiner {
public:
	PatternMiner();

	~PatternMiner();

	void operator()(const Problem& problem, const vector<Solution>& solutions);

	vector<int> ProposePattern(const Problem& problem, int oid, double support);

private:
	class PatternNode {
	public:
		void GetSuccessor(vector<int>& full_pattern, double support, int depth);
		int opNumber;
		list<pair<PatternNode, double>> successors;
	};

private:
	unsigned int solCount;
	unsigned int maxDepth;
	PatternNode orderedPatterns;
};

#endif // PATTERNS_MINER_
