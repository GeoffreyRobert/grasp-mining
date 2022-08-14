#include <cassert>

#include "solution.h"

using std::vector;

OpUpdate& operator++(OpUpdate& up)
{
  return up = static_cast<OpUpdate>(static_cast<int>(up) + 1);
}
OpUpdate& operator--(OpUpdate& up)
{
  return up = static_cast<OpUpdate>(static_cast<int>(up) - 1);
}

Solution::Solution(const Problem& problem_)
  : problem(problem_)
  , startDate(problem_.size)
  , endDate(problem_.size)
  , macParent(problem_.size, problem.OriginOp)
  , macChild(problem_.size, problem.FinalOp)
  , isCritMachine(problem_.size)
{
}

Solution::Solution(const Solution& other)
  : problem(other.problem)
  , startDate(other.startDate)
  , endDate(other.endDate)
  , macParent(other.macParent)
  , macChild(other.macChild)
  , isCritMachine(other.isCritMachine)
{
}

// Move assignment operator.
Solution& Solution::operator=(Solution&& other) noexcept
{
  if (this != &other && &problem == &other.problem) {
    makespan = other.makespan;
    criticalOp = other.criticalOp;

    startDate = std::move(other.startDate);
    endDate = std::move(other.endDate);
    macParent = std::move(other.macParent);
    macChild = std::move(other.macChild);
    isCritMachine = std::move(other.isCritMachine);
  }
  return *this;
}

// Copy assignment operator.
Solution& Solution::operator=(const Solution& other)
{
  if (this != &other && &problem == &other.problem) {
    makespan = other.makespan;
    criticalOp = other.criticalOp;

    startDate = other.startDate;
    endDate = other.endDate;
    macParent = other.macParent;
    macChild = other.macChild;
    isCritMachine = other.isCritMachine;
  }
  return *this;
}

std::tuple<OperationId, int, bool> Solution::GetOperationScheduling(OperationId oid)
{
  // initialisations durées+parent
  OperationId parent_in_job = problem.prevOperation[oid];
  int date_job = endDate[parent_in_job];

  OperationId parent_on_mac = macParent[oid];
  int date_mac = endDate[parent_on_mac];

  OperationId parent;
  int start_date;
  bool is_on_mac;
  if (date_job < date_mac) { // si la date disj est superieure
    parent = parent_on_mac;
    start_date = date_mac;
    is_on_mac = true;
  } else {
    parent = parent_in_job;
    start_date = date_job;
    is_on_mac = false;
  }
  startDate[oid] = start_date;
  endDate[oid] = start_date + problem.timeOnMachine[oid];
  isCritMachine[oid] = is_on_mac;

  return {parent, start_date, is_on_mac};
}

void Solution::AddOperation(OperationId oid)
{
  macChild[macParent[oid]] = oid;

  if (endDate[oid] > makespan) {
    criticalOp = oid;
    makespan = endDate[oid];
    macParent[problem.FinalOp] = criticalOp;
    startDate[problem.FinalOp] = makespan;
    endDate[problem.FinalOp] = makespan;
  }

  // update all operations on the same machine that were not yet added to the solution
  MachineId mac = problem.machineNumber[oid];
  for (OperationId oid_updt : problem.operationsOnMachine[mac])
  {
    // test if the operation was added
    if (oid_updt != oid && macChild[oid_updt] == problem.FinalOp)
      macParent[oid_updt] = oid;
  }
}

unsigned Solution::DoChanges(
  vector<OpUpdate>& is_changed, vector<int>& new_start_date,
  vector<int>& new_end_date, vector<bool>& new_is_crit_mac)
{
  for (OperationId oid = 0; oid < problem.size; ++oid) {
    if (is_changed[oid] == OpUpdate::Changed) {
      startDate[oid] = new_start_date[oid];
      endDate[oid] = new_end_date[oid];
      isCritMachine[oid] = new_is_crit_mac[oid];
    }
  }

  // Search the new critical op
  OperationId last_op_of_job = problem.nMac;
  makespan = 0;
  for (JobId j = 0; j < problem.nJob; ++j) {
    if (endDate[last_op_of_job] > makespan) {
      makespan = endDate[last_op_of_job];
      criticalOp = last_op_of_job;
    }
    last_op_of_job += problem.nMac;
  }

  return criticalOp;
}

void Solution::SwapOperations(OperationId parent, OperationId child)
{
  // inversion des deux operations
  OperationId swap_predecessor = macParent[parent]; // predecessor of both swapped ops on machine
  macParent[child] = swap_predecessor;
  macChild[swap_predecessor] = child;

  OperationId swap_successor = macChild[child];
  macChild[parent] = swap_successor;
  macParent[swap_successor] = parent;

  macParent[parent] = child;
  macChild[child] = parent;
}
