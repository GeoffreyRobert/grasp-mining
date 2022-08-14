#ifndef SOLUTION_
#define SOLUTION_

#include <tuple>
#include <vector>

#include "data/problem.h"

using std::tuple;
using std::vector;

enum OpUpdate {
  Unchanged = 0,
  ToChange = 1,
  Changed = 2,
  ChangedToChange = 3
};
OpUpdate& operator++(OpUpdate&);
OpUpdate& operator--(OpUpdate&);

class InvalidScheduling : std::logic_error {
public:
  InvalidScheduling(const string& what);
};

class Solution {
public:
  Solution(const Problem& problem);
  Solution(const Solution& other);
  Solution& operator=(const Solution& other);
  Solution& operator=(Solution&& other) noexcept;

  void Initialize(
      vector<int>&& startDate
    , vector<int>&& endDate
    , vector<OperationId>&& macParent
    , vector<OperationId>&& macChild
    , vector<bool>&& isCritMachine);

  // Getters
  OperationId ParentOnMachine(OperationId oid) const;
  OperationId ChildOnMachine(OperationId oid) const;
  bool IsCriticalOnMachine(OperationId oid) const;

  // Get a scheduling for the operation and cache it
  tuple<OperationId, int, bool> GetOperationScheduling(OperationId oid);

  void AddOperation(OperationId oid);
  void SwapOperations(OperationId parent, OperationId child);

  unsigned DoChanges(
    vector<OpUpdate>& is_changed, vector<int>& new_start_date,
    vector<int>& new_end_date, vector<bool>& new_is_crit_mac);

  const Problem& problem;

  // Plannification de la solution
  int makespan = 0;
  OperationId criticalOp = 0;

  vector<int> startDate; // date de début de chaque opération
  vector<int> endDate; // date de fin de chaque operation

private:
  void CheckScheduling(OperationId oid) const;
  void CheckCycle(OperationId oid) const;
  void CheckNoChild(OperationId oid) const;

  vector<OperationId> macParent; // parent sur la machine
  vector<OperationId> macChild; // successeur(s) sur la machine
  vector<bool> isCritMachine; // parent critique sur la machine
};
#endif
