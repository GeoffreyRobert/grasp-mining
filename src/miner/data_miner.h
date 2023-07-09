#ifndef DATA_MINER_H_
#define DATA_MINER_H_

#include <vector>

#include "data/typedefs.h"
#include "solver/solver_module.h"

using std::vector; using std::pair;

class Solution;

class DataMiner : public SolverModule {
public:
  using SolverModule::SolverModule;
  virtual ~DataMiner() {};
  virtual void operator()(const vector<Solution>&) = 0;
  virtual vector<vector<pair<OperationId, OperationId>>> GetItemsets() = 0;
};
#endif // DATA_MINER_H_
