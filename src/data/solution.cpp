#include <cassert>
#include <limits>
#include <stdexcept>

#include "data/solution.h"

using std::vector;

OpUpdate& operator++(OpUpdate& up)
{
  return up = static_cast<OpUpdate>(static_cast<int>(up) + 1);
}
OpUpdate& operator--(OpUpdate& up)
{
  return up = static_cast<OpUpdate>(static_cast<int>(up) - 1);
}


IncompatibleScheduling::IncompatibleScheduling()
  : std::logic_error("Ordering on machine and start dates are inverted")
{}


Solution::Solution(const Problem& problem_)
  : problem(problem_)
  , startDate(problem_.size, std::numeric_limits<int>::max())
  , endDate(problem_.size, std::numeric_limits<int>::max())
  , macParent(problem_.size, problem.OriginOp)
  , macChild(problem_.size, problem.FinalOp)
  , isCritMachine(problem_.size)
{
  startDate[problem.OriginOp] = 0;
  endDate[problem.OriginOp] = 0;
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

void Solution::Initialize(
    vector<int>&& startDate_
  , vector<int>&& endDate_
  , vector<OperationId>&& macParent_
  , vector<OperationId>&& macChild_
  , vector<bool>&& isCritMachine_)
{
  startDate = std::move(startDate_);
  endDate_ = std::move(endDate);
  macParent_ = std::move(macParent);
  macChild_ = std::move(macChild);
  isCritMachine_ = std::move(isCritMachine);
}

OperationId Solution::ParentOnMachine(OperationId oid) const
{
  // to remove
  OperationId parent_on_machine = macParent[oid];
  int parent_end_date = endDate[parent_on_machine];
  int current_start_date = startDate[oid];
  if (current_start_date < parent_end_date)
  {
    throw IncompatibleScheduling();
  }

  return macParent[oid];
}

OperationId Solution::ChildOnMachine(OperationId oid) const
{
  // to remove
  OperationId child_on_machine = macChild[oid];
  int child_start_date = startDate[child_on_machine];
  int current_end_date = endDate[oid];
  if (child_start_date < current_end_date)
  {
    throw IncompatibleScheduling();
  }

  return macChild[oid];
}

bool Solution::IsCriticalOnMachine(OperationId oid) const
{
  return isCritMachine[oid];
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
  // to remove
  OperationId parent_on_machine = macParent[oid];
  int parent_end_date = endDate[parent_on_machine];
  int current_end_date = endDate[oid];
  if (current_end_date < parent_end_date)
  {
    throw IncompatibleScheduling();
  }

  macChild[macParent[oid]] = oid;

  if (endDate[oid] > makespan) {
    criticalOp = oid;
    makespan = endDate[oid];
    macParent[problem.FinalOp] = criticalOp;
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

  // to remove
  for (OperationId oid = 0; oid < problem.size; ++oid) {
    if (is_changed[oid] == OpUpdate::Changed) {
      OperationId parent_on_machine = macParent[oid];
      int parent_end_date = endDate[parent_on_machine];
      int current_end_date = endDate[oid];
      if (current_end_date < parent_end_date)
      {
        throw IncompatibleScheduling();
      }
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
