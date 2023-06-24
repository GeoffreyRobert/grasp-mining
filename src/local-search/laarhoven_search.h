#ifndef LAARHOVEN_H_
#define LAARHOVEN_H_

#include <queue>

#include "boost/circular_buffer.hpp"

#include "data/problem.h"
#include "data/solution.h"
#include "local_search.h"

using std::deque;
using std::vector;

// TODO: introduce policy for CheckAndSwap and erase the type
// template<class OpsMover>
class LaarhovenSearch : public LocalSearch {
public:
  LaarhovenSearch(const Problem&);

  // Recherche locale le long du chemin critique
  Solution& operator()(Solution& solution) override;

private:
  bool SwapAndEvaluate(Solution&, OperationId parent, OperationId child);
  bool SwapAndUpdateOps(OperationId parent, OperationId child, int makespan);
  bool UpdateOperation(OperationId oid, int makespan);

  // temporary solution to try out swaps
  Solution draft_solution;

  // stockage des operations à déplacer
  boost::circular_buffer<OperationId> ops_to_move;
};

#endif // LAARHOVEN_H_
