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


InvalidScheduling::InvalidScheduling(const string& what)
  : std::logic_error(what)
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
  , makespan(other.makespan)
  , criticalOp(other.criticalOp)
  , startDate(other.startDate)
  , endDate(other.endDate)
  , macParent(other.macParent)
  , macChild(other.macChild)
  , isCritMachine(other.isCritMachine)
{
}

Solution::Solution(Solution&& other) noexcept
  : problem(other.problem)
  , makespan(std::exchange(other.makespan, 0))
  , criticalOp(std::exchange(other.criticalOp, 0))
  , startDate(std::move(other.startDate))
  , endDate(std::move(other.endDate))
  , macParent(std::move(other.macParent))
  , macChild(std::move(other.macChild))
  , isCritMachine(std::move(other.isCritMachine))
{
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

// Move assignment operator.
Solution& Solution::operator=(Solution&& other) noexcept
{
  if (this != &other && &problem == &other.problem) {
    makespan = std::exchange(other.makespan, 0);
    criticalOp = std::exchange(other.criticalOp, 0);

    startDate = std::move(other.startDate);
    endDate = std::move(other.endDate);
    macParent = std::move(other.macParent);
    macChild = std::move(other.macChild);
    isCritMachine = std::move(other.isCritMachine);
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
  endDate = std::move(endDate_);
  macParent = std::move(macParent_);
  macChild = std::move(macChild_);
  isCritMachine = std::move(isCritMachine_);
}

OperationId Solution::ParentOnMachine(OperationId oid) const
{
  return macParent[oid];
}

OperationId Solution::ChildOnMachine(OperationId oid) const
{
  return macChild[oid];
}

bool Solution::IsCriticalOnMachine(OperationId oid) const
{
  return isCritMachine[oid];
}

int Solution::GetOperationScheduling(OperationId oid)
{
  // initialisations durées+parent
  OperationId parent_in_job = problem.prevOperation[oid];
  int date_job = endDate[parent_in_job];

  OperationId parent_on_mac = macParent[oid];
  int date_mac = endDate[parent_on_mac];

  int start_date;
  bool is_on_mac;
  if (date_job < date_mac) { // si la date disj est superieure
    start_date = date_mac;
    is_on_mac = true;
  } else {
    start_date = date_job;
    is_on_mac = false;
  }
  int end_date = start_date + problem.timeOnMachine[oid];

  startDate[oid] = start_date;
  endDate[oid] = end_date;
  isCritMachine[oid] = is_on_mac;

  return end_date;
}

void Solution::AddOperation(OperationId oid)
{
  // this is a necesary protection, if not fulfilled, could create nasty cycles
  if (macChild[oid] != problem.FinalOp)
    throw InvalidScheduling("Cannot add operation that was already added");

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
    {
      macParent[oid_updt] = oid;
    }
  }

  CheckCycle(oid);
}

int Solution::SwapOperations(OperationId parent, OperationId child)
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

  // update schedule
  GetOperationScheduling(child);
  int end_date = GetOperationScheduling(parent);

  CheckCycle(parent);
  CheckCycle(child);

  return end_date;
}

int Solution::RescheduleOperation(OperationId oid)
{
  int end_date = GetOperationScheduling(oid);

  if (end_date > makespan) {
    criticalOp = oid;
    makespan = end_date;
    macParent[problem.FinalOp] = criticalOp;
  }
  else if (oid == criticalOp && end_date < makespan) {
    UpdateMakespan();
  }

  CheckCycle(oid);

  return end_date;
}

bool Solution::TryResetOperation(OperationId oid)
{
  if (oid == problem.FinalOp)
    return false;

  // the "is set" invariant as per AddOperation
  if (endDate[oid] == std::numeric_limits<int>::max())
    return false;

  endDate[oid] = std::numeric_limits<int>::max();
  return true;
}

void Solution::UpdateMakespan()
{
  OperationRank last_rank = problem.nMac - 1;
  makespan = 0;
  for (JobId jid = 0; jid < problem.nJob; ++jid) {
    OperationId oid = problem.operationNumber[jid][last_rank];
    int end_date = endDate[oid];
    if (end_date > makespan && end_date != std::numeric_limits<int>::max()) {
      makespan = end_date;
      criticalOp = oid;
    }
  }
}

void Solution::CheckScheduling(OperationId oid) const
{
  OperationId parent_on_machine = macParent[oid];
  int parent_end_date = endDate[parent_on_machine];
  int current_end_date = endDate[oid];
  if (current_end_date < parent_end_date)
  {
    throw InvalidScheduling("Machine ordering conflicts with date assignments");
  }
}

void Solution::CheckCycle(OperationId oid) const
{
  OperationId parent = macParent[oid];
  while (parent != problem.OriginOp)
  {
    if (parent == oid)
      throw InvalidScheduling("Cycle detected");
    parent = macParent[parent];
  }

  OperationId child = macChild[oid];
  while (child != problem.FinalOp)
  {
    if (child == oid)
      throw InvalidScheduling("Cycle detected");
    child = macChild[child];
  }
}

void Solution::CheckNoChild(OperationId oid) const
{
  if (oid != problem.OriginOp && macChild[oid] != problem.FinalOp)
    throw InvalidScheduling("Operation should not have a child assigned here");
}
