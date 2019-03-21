#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

class PatternMiner {
public:
	PairPatterns();

	~PairPatterns();

	void BuildGraph(const Problem& problem, const vector<BierwirthSequence>& solutions);

	int ProposeNextOp(const Problem& problem, int oid);

};

#endif // PATTERNS_MINER_
