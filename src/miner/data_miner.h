#ifndef DATA_MINER_H_
#define DATA_MINER_H_

#include <vector>

#include "data/problem.h"
#include "data/solution.h"

using std::vector;

class DataMiner {
public:
	virtual void operator()(const Problem&, const vector<Solution>&) = 0;
	virtual vector<int> ProposePattern(const Problem&, int operation, double support) = 0;

};
#endif // DATA_MINER_H_
