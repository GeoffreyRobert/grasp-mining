#ifndef SOLUTION_
#define SOLUTION_

#include <functional>
#include <vector>

#include "problem.h"

using std::vector;

enum OpUpdate {
  Unchanged = 0,
  ToChange = 1,
  Changed = 2,
  ChangedToChange = 3
};
OpUpdate& operator++(OpUpdate&);
OpUpdate& operator--(OpUpdate&);

class Solution {
public:
  Solution(const Problem& problem);
  Solution(const Solution& other);
  Solution& operator=(const Solution& other);
  Solution& operator=(Solution&& other) noexcept;

  // Get a scheduling for the operation and cache it
  std::tuple<OperationId, int, bool> GetOperationScheduling(OperationId oid);

  void AddOperation(OperationId oid);
  void SwapOperations(OperationId parent, OperationId child);

  unsigned DoChanges(
    vector<OpUpdate>& is_changed, vector<int>& new_start_date,
    vector<int>& new_end_date, vector<bool>& new_is_crit_mac);

  const Problem& problem;

  // Plannification de la solution
  int makespan = 0;
  OperationId criticalOp = 0;

public:
  vector<int> startDate; // date de début de chaque opération
  vector<int> endDate; // date de fin de chaque operation

  vector<OperationId> macParent; // parent sur la machine
  vector<OperationId> macChild; // successeur(s) sur la machine
  vector<bool> isCritMachine; // parent critique sur la machine
};
#endif
