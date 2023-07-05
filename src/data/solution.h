#ifndef SOLUTION_
#define SOLUTION_

#include <string>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "data/typedefs.h"

using std::string;
using std::tuple;
using std::vector;

enum class ParentType : char
{
  None = 0,
  Job = 1 << 0,
  Machine = 1 << 1,
};
inline ParentType operator|(ParentType a, ParentType b);
inline ParentType operator&(ParentType a, ParentType b);

enum OpUpdate {
  Unchanged = 0,
  ToChange = 1,
  Changed = 2,
  ChangedToChange = 3
};
OpUpdate& operator++(OpUpdate&);
OpUpdate& operator--(OpUpdate&);

class Problem;

class InvalidScheduling : std::logic_error {
public:
  InvalidScheduling(const string& what);
};

class Solution {
public:
  Solution(const Problem& problem);
  Solution(const Solution& other);
  Solution(Solution&& other) noexcept;
  Solution& operator=(const Solution& other);
  Solution& operator=(Solution&& other) noexcept;

  void Initialize(
      vector<int>&& startDate
    , vector<int>&& endDate
    , vector<OperationId>&& macParent
    , vector<OperationId>&& macChild);

  // Getters
  int Makespan();
  OperationId CriticalOp();

  OperationId ParentOnMachine(OperationId oid) const;
  OperationId ChildOnMachine(OperationId oid) const;
  bool IsCriticalOnMachine(OperationId oid) const;
  bool IsCriticalOnJob(OperationId oid) const;

  // Get a scheduled end date for the operation and cache it
  int ScheduleOperation(OperationId oid);
  int StartDate(OperationId oid);
  int EndDate(OperationId oid);

  void AddOperation(OperationId oid);
  OperationId SwapOperations(OperationId parent, OperationId child);
  int RescheduleOperation(OperationId oid);
  bool TryResetOperation(OperationId oid, ParentType);

  const Problem& problem;

private:
  void UpdateMakespan();
  void CheckScheduling(OperationId oid) const;
  bool HasCycle(OperationId oid) const;
  bool HasChild(OperationId oid) const;

  vector<int> startDate; // date de début de chaque opération
  vector<int> endDate; // date de fin de chaque operation

  vector<OperationId> macParent; // parent sur la machine
  vector<OperationId> macChild; // successeur(s) sur la machine
  vector<ParentType> isCritical; // parent critique sur la machine
};
#endif
