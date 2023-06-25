#ifndef DATA_MINER_H_
#define DATA_MINER_H_

#include <vector>

#include "data/typedefs.h"
#include "solver/solver_module.h"

using std::vector;

class Solution;

class DataMiner : public SolverModule {
public:
  using SolverModule::SolverModule;
  virtual ~DataMiner() {};
  virtual void operator()(const vector<Solution>&) = 0;
  virtual vector<int> ProposePattern(OperationId operation, double support) = 0;
};
#endif // DATA_MINER_H_
