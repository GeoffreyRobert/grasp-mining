#ifndef TRANSACTION_ENCODER_
#define TRANSACTION_ENCODER_

#include <utility>

#include "data/typedefs.h"
#include "solver/solver_module.h"

class TransactionEncoder : public SolverModule
{
public:
  using SolverModule::SolverModule;
  int OperationPairToItem(OperationId prev_oid, OperationId oid) const;
  std::pair<OperationId, OperationId> ItemToOperationPair(int itid) const;
};

#endif // TRANSACTION_ENCODER_
