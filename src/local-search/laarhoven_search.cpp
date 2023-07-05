#include <utility>

#include "laarhoven_search.h"

using std::deque;

LaarhovenSearch::LaarhovenSearch(const Problem& problem)
  : LocalSearch(problem)
  , draft_solution(problem)
{
  ops_to_move.reserve(ref_pb.size);
}

Solution& LaarhovenSearch::operator()(Solution& solution)
{
  OperationId operation = solution.CriticalOp(); // op. considérée pour relocation
  OperationId parent = solution.ParentOnMachine(operation); // parent de l'operation

  // remonter le chemin critique
  do {
    if (!solution.IsCriticalOnMachine(operation)) {
      // on ignore deux ops. dans le meme job
      operation = ref_pb.prevOperation[operation];
    } else {
      // on inverse deux ops. sur le chemin critique si réduction du makespan
      if (SwapAndEvaluate(solution, parent, operation)) {
        operation = solution.CriticalOp();
        hit_count++; // TODO: sous preproc. pour debug
      } else {
        // on continue sur le chemin critique
        operation = parent;
      }
    }
    parent = solution.ParentOnMachine(operation);
  } while (parent != ref_pb.OriginOp);
  return solution;
}

/** En charge de la sélection des opérations à examiner
 *  Returns the id of the operation that was updated and is scheduled the latest
 **/
bool LaarhovenSearch::SwapAndEvaluate(
    Solution& solution, OperationId parent, OperationId child)
{
  // copy the solution to a draft
  draft_solution = solution;

  // invert parent / child on the critical path and get the new parent of the pair
  OperationId new_parent = draft_solution.SwapOperations(parent, child);
  ops_to_move.push_back(new_parent);

  // stack vs queue en termes de perfs ? stack: localité, queue: moins de doubles accès
  while (!ops_to_move.empty()) {
    // get an operation to process
    unsigned oid = ops_to_move.back();
    ops_to_move.pop_back();

    // schedule & check that the draft is improving on the base solution
    int end_date = draft_solution.RescheduleOperation(oid);
    if (end_date >= solution.Makespan()) {
      ops_to_move.clear();
      return false;
    }

    // add successors
    // successors on machine is added first to be processed last
    // this works very well (10x better) when there are many jobs and few machines
    OperationId child_on_mac = draft_solution.ChildOnMachine(oid);
    if (draft_solution.TryResetOperation(child_on_mac, ParentType::Job))
      ops_to_move.push_back(child_on_mac);

    OperationId child_in_job = ref_pb.nextOperation[oid];
    if (draft_solution.TryResetOperation(child_in_job, ParentType::Machine))
      ops_to_move.push_back(child_in_job);

    // the ordering of the above should depend on whether there more jobs
    // than machines or vice-versa
  }

  // no-copy replacement of the solution by the draft
  std::swap(solution, draft_solution);
  return true;
}
