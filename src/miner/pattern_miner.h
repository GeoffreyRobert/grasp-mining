#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include <list>
#include <utility>

#include "data_miner.h"

using std::list; using std::pair;

class PatternMiner : public DataMiner {
public:
	PatternMiner();

	virtual void operator()(const vector<Solution>& solutions);

	virtual vector<int> ProposePattern(OperationId oid, double support);

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
