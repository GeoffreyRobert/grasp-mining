#ifndef PATTERNS_MINER_
#define PATTERNS_MINER_

#include "data_miner.h"

using std::pair;

class TransactionEncoder;

class PatternMiner : public DataMiner {
public:
  PatternMiner(const Problem&, double support, const TransactionEncoder& encoder);
  virtual void operator()(const vector<Solution>& solutions);

private:
  std::vector<int> SolutionToVec(const Solution&) const;

  const TransactionEncoder& _encoder;
  const double _support;
};

#endif // PATTERNS_MINER_
