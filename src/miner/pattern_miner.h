#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include <list>
#include <utility>

#include "data_miner.h"

using std::list; using std::pair;

class TransactionEncoder;

class PatternMiner : public DataMiner {
public:
	PatternMiner(const Problem&, double support, const TransactionEncoder& encoder);

	virtual void operator()(const vector<Solution>& solutions);

	virtual vector<int> ProposePattern(OperationId oid, double support);

private:
  std::vector<int> SolutionToVec(const Solution&) const;

	class PatternNode {
	public:
		void GetSuccessor(vector<int>& full_pattern, double support, int depth);
		int opNumber;
		list<pair<PatternNode, double>> successors;
	};

private:
  const TransactionEncoder& _encoder;
  const double _support;
};

#endif // PATTERNS_MINER_
