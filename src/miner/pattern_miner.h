#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include "data_miner.h"

class TransactionEncoder;
class SolutionFilter;
class Transaction;

class PatternMiner : public DataMiner
{
public:
  PatternMiner(
      const Problem&
    , const TransactionEncoder&
    , const SolutionFilter&
    , double support);
  void operator()(vector<Solution>& solutions) override;
  const vector<pair<OperationId, OperationId>>& GetItemset() override;

private:
  vector<Transaction> SolutionsToVectors(const vector<Solution>& solutions) const;
  const TransactionEncoder& _encoder;
  const SolutionFilter& _filter;
  const double _support;
  const double _itemset_ratio = 0.2;
  const double _support_mult = 0.8;

  unsigned _min_support;
  unsigned _max_support;
  vector<unsigned> _length_distrib;
  vector<vector<pair<OperationId, OperationId>>> _itemsets;
  vector<vector<pair<OperationId, OperationId>>>::iterator _iter;
};

#endif // PATTERNS_MINER_
