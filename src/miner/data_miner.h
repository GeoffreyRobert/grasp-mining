#ifndef DATA_MINER_H_
#define DATA_MINER_H_

#include <vector>

#include "data/problem.h"
#include "data/solution.h"
#include "solver/solver_module.h"

using std::vector;

class DataMiner : public SolverModule {
public:
    using SolverModule::SolverModule;
	virtual void operator()(const vector<Solution>&) = 0;
	virtual vector<int> ProposePattern(int operation, double support) = 0;
};
#endif // DATA_MINER_H_
