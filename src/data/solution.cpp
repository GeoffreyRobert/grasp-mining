#include <cassert>
#include <limits>
#include <stdexcept>

#include "data/solution.h"
#include "data/problem.h"

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
  startDate[problem.OriginOp] = std::numeric_limits<int>::min();
  endDate[problem.OriginOp] = 0;
  startDate[problem.FinalOp] = 0;
  endDate[problem.FinalOp] = std::numeric_limits<int>::max();
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

Solution::Solution(Solution&& other) noexcept
  : problem(other.problem)
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

int Solution::Makespan()
{
  return startDate[problem.FinalOp];
}

OperationId Solution::CriticalOp()
{
  return macParent[problem.FinalOp];
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

int Solution::ScheduleOperation(OperationId oid)
{
  assert(oid != problem.FinalOp && "schedule attempt on final operation");

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

int Solution::StartDate(OperationId oid)
{
  return startDate[oid];
}

int Solution::EndDate(OperationId oid)
{
  return endDate[oid];
}

void Solution::AddOperation(OperationId oid)
{
  // this is a necesary protection, if not fulfilled, could create nasty cycles
  if (macChild[oid] != problem.FinalOp)
    throw InvalidScheduling("Cannot add operation that was already added");

  macChild[macParent[oid]] = oid;

  if (endDate[oid] > Makespan()) {
    startDate[problem.FinalOp] = endDate[oid];
    macParent[problem.FinalOp] = oid;
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

  assert(!HasCycle(oid) && "add operation created a cycle");
}

int Solution::SwapOperations(OperationId parent, OperationId child)
{
  // inversion des deux operations
  OperationId swap_predecessor = macParent[parent]; // predecessor of both swapped ops on machine
  macParent[child] = swap_predecessor;
  macChild[swap_predecessor] = child;

  OperationId swap_successor = macChild[child];
  macChild[parent] = swap_successor;
  // only reassign FinalOp if the child was the previous critical op
  if (swap_successor != problem.FinalOp || child == CriticalOp())
  {
    macParent[swap_successor] = parent;
  }

  macParent[parent] = child;
  macChild[child] = parent;

  // update schedule
  ScheduleOperation(child);
  int end_date = RescheduleOperation(parent);

  assert(!HasCycle(parent) && "swap created a cycle with parent operation");
  assert(!HasCycle(child) && "swap created a cycle with child operation");

  return end_date;
}

int Solution::RescheduleOperation(OperationId oid)
{
  assert(oid != problem.FinalOp && "reschedule attempt on final operation");

  int end_date = ScheduleOperation(oid);

  if (end_date > Makespan()) {
    startDate[problem.FinalOp] = endDate[oid];
    macParent[problem.FinalOp] = oid;
  }
  else if (oid == CriticalOp() && end_date < Makespan()) {
    UpdateMakespan();
  }

  assert(!HasCycle(oid) && "reschedule operation created a cycle");

  return end_date;
}

bool Solution::TryResetOperation(OperationId oid)
{
  // the "is set" invariant as per AddOperation
  if (endDate[oid] == std::numeric_limits<int>::max())
    return false;

  endDate[oid] = std::numeric_limits<int>::max();
  return true;
}

void Solution::UpdateMakespan()
{
  startDate[problem.FinalOp] = 0;
  for (JobId jid = 0; jid < problem.nJob; ++jid) {
    OperationId oid = problem.operationNumber[jid].back();
    int end_date = endDate[oid];
    if (end_date > Makespan() && end_date != std::numeric_limits<int>::max()) {
      startDate[problem.FinalOp] = end_date;
      macParent[problem.FinalOp] = oid;
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

bool Solution::HasCycle(OperationId oid) const
{
  OperationId parent = macParent[oid];
  while (parent != problem.OriginOp)
  {
    if (parent == oid)
      return true;
    parent = macParent[parent];
  }

  OperationId child = macChild[oid];
  while (child != problem.FinalOp)
  {
    if (child == oid)
      return true;
    child = macChild[child];
  }

  return false;
}

bool Solution::HasChild(OperationId oid) const
{
  return oid != problem.OriginOp && macChild[oid] != problem.FinalOp;
}
