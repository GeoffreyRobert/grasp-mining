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
  std::pair<OperationId, int> SwapAndEvaluate(Solution&, OperationId parent, OperationId child);
  void SwapAndUpdateOps(Solution& sol, unsigned parent, unsigned child);
  void CancelSwap(Solution& sol, unsigned parent, unsigned child);
  void UpdateOp(const Solution& sol, unsigned oid);
  void AddSuccessors(const Solution& sol, unsigned oid);

  // stockage des operations à déplacer
  boost::circular_buffer<OperationId> ops_to_move;

  // stockage des modifications de la solution
  vector<OpUpdate> is_changed;
  vector<int> new_start_date;
  vector<int> new_end_date;
  vector<bool> new_is_crit_mac;
};

#endif // LAARHOVEN_H_
