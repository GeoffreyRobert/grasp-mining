#ifndef EMPTY_MINER_H_
#define EMPTY_MINER_H_

#include "data_miner.h"

class EmptyMiner : public DataMiner {
public:
	void operator()(const Problem&, const vector<Solution>&);
	vector<int> ProposePattern(const Problem&, int operation, double support);

    void ResourcesAlloc(const Problem&);
};
#endif // EMPTY_MINER_H_
