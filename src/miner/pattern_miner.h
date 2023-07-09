#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include "data_miner.h"

class TransactionEncoder;
class Transaction;

class PatternMiner : public DataMiner {
public:
  PatternMiner(const Problem&, double support, const TransactionEncoder& encoder);
  void operator()(const vector<Solution>& solutions) override;
  vector<vector<pair<OperationId, OperationId>>> GetItemsets() override;

private:
  vector<Transaction> SolutionsToVectors(const vector<Solution>& solutions) const;
  const TransactionEncoder& _encoder;
  vector<vector<pair<OperationId, OperationId>>> _itemsets;
  const double _support;
  const double _itemset_ratio = 0.2;
  const double _support_incr = 0.05;
};

#endif // PATTERNS_MINER_
