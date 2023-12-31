#include <cassert>
#include <limits>
#include <numeric>
#include <stdexcept>

#include "data/solution.h"
#include "data/problem.h"

using std::vector;

ParentType operator|(ParentType a, ParentType b)
{
    return static_cast<ParentType>(static_cast<char>(a) | static_cast<char>(b));
}
ParentType operator&(ParentType a, ParentType b)
{
    return static_cast<ParentType>(static_cast<char>(a) & static_cast<char>(b));
}

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
  , startDate(problem_.size, std::numeric_limits<int>::min())
  , endDate(problem_.size, std::numeric_limits<int>::max())
  , macParent(problem_.size, problem.OriginOp)
  , macChild(problem_.size, problem.FinalOp)
  , isCritical(problem_.size)
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
  , isCritical(other.isCritical)
{
}

Solution::Solution(Solution&& other) noexcept
  : problem(other.problem)
  , startDate(std::move(other.startDate))
  , endDate(std::move(other.endDate))
  , macParent(std::move(other.macParent))
  , macChild(std::move(other.macChild))
  , isCritical(std::move(other.isCritical))
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
    isCritical = other.isCritical;
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
    isCritical = std::move(other.isCritical);
  }
  return *this;
}

bool Solution::operator<(const Solution& other) const noexcept
{
  return Makespan() < other.Makespan();
}

void Solution::Initialize(
    vector<int>&& startDate_
  , vector<int>&& endDate_
  , vector<OperationId>&& macParent_
  , vector<OperationId>&& macChild_)
{
  startDate = std::move(startDate_);
  endDate = std::move(endDate_);
  macParent = std::move(macParent_);
  macChild = std::move(macChild_);
  isCritical.resize(problem.size);
  isCritical[problem.OriginOp] = ParentType::None;
  isCritical[problem.FinalOp] = ParentType::Machine;
  for (OperationId oid = problem.OriginOp + 1; oid < problem.FinalOp; ++oid)
  {
    OperationId parent_in_job = problem.prevOperation[oid];
    int date_job = endDate[parent_in_job];
    OperationId parent_on_mac = macParent[oid];
    int date_mac = endDate[parent_on_mac];

    ParentType& is_critical = isCritical[oid] = ParentType::None;
    // go through both if to set critical parent data
    if (date_job <= date_mac)
      is_critical = is_critical | ParentType::Machine;
    if (date_mac <= date_job)
      is_critical = is_critical | ParentType::Job;
  }
}

int Solution::Makespan() const
{
  return startDate[problem.FinalOp];
}

OperationId Solution::CriticalOp() const
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
  return (isCritical[oid] & ParentType::Machine) != ParentType::None;
}

bool Solution::IsCriticalOnJob(OperationId oid) const
{
  return (isCritical[oid] & ParentType::Job) != ParentType::None;
}

int Solution::ScheduleOperation(OperationId oid)
{
  assert(oid != problem.FinalOp && "schedule attempt on final operation");

  // initialisations durées+parent
  OperationId parent_in_job = problem.prevOperation[oid];
  int date_job = endDate[parent_in_job];

  OperationId parent_on_mac = macParent[oid];
  int date_mac = endDate[parent_on_mac];

  assert(IsScheduled(parent_in_job) && "parent in job is not scheduled yet");
  assert(IsScheduled(parent_on_mac) && "parent on machine is not scheduled yet");

  int start_date;
  ParentType is_critical{ParentType::None};
  // go through both if to set critical parent data
  if (date_job <= date_mac)
  {
    start_date = date_mac;
    is_critical = is_critical | ParentType::Machine;
  }
  if (date_mac <= date_job)
  {
    start_date = date_job;
    is_critical = is_critical | ParentType::Job;
  }
  int end_date = start_date + problem.timeOnMachine[oid];

  startDate[oid] = start_date;
  endDate[oid] = end_date;
  isCritical[oid] = is_critical;

  return end_date;
}

int Solution::StartDate(OperationId oid) const
{
  return startDate[oid];
}

int Solution::EndDate(OperationId oid) const
{
  return endDate[oid];
}

void Solution::AddOperation(OperationId oid)
{
  // this is a necesary protection, if not fulfilled, could create nasty cycles
  if (macChild[oid] != problem.FinalOp)
    throw InvalidScheduling("Cannot add operation that was already added");

  // if the scheduling was not cached, compute it
  if (!IsScheduled(oid))
    ScheduleOperation(oid);

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

OperationId Solution::SwapOperations(OperationId parent, OperationId child)
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

  assert(!HasCycle(parent) && "swap created a cycle with parent operation");
  assert(!HasCycle(child) && "swap created a cycle with child operation");

  // new parent
  return child;
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

bool Solution::IsScheduled(OperationId oid) const
{
  return endDate[oid] != std::numeric_limits<int>::max();
}

// test if an operation can be rescheduled and tag it if so
bool Solution::TryResetOperation(OperationId oid)
{
  // check whether it's already reset to be rescheduled
  // the "is scheduled" invariant as per AddOperation
  if (!IsScheduled(oid))
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
    if (end_date > Makespan()) {
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

double mean(const vector<Solution>& solution_set)
{
  double makespan_mean = std::accumulate(
      begin(solution_set), end(solution_set), 0.0
      , [](double acc, const auto& sol) { return acc + sol.Makespan(); });
  makespan_mean /= static_cast<double>(solution_set.size());
  return makespan_mean;
}
