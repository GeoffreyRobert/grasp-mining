#include <deque>
#include <utility>

#include "laarhoven_search.h"

using std::deque;

LaarhovenSearch::LaarhovenSearch(const Problem& problem)
  : LocalSearch(problem)
  , draft_solution(problem)
{
  ops_to_move.set_capacity(ref_pb.size);
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
        // no-copy replacement of the solution by the draft
        solution = std::move(draft_solution);
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
  // cas qui allonge forcément le chemin critique
  if (solution.startDate[parent] < solution.endDate[ref_pb.prevOperation[child]]) {
    return false;
  }

  // copy the solution to a draft
  draft_solution = solution;

  // inversion des 2 opérations sur le chemin critique
  int new_end_date = SwapAndUpdateOps(parent, child);
  if (new_end_date >= solution.Makespan()) {
    ops_to_move.clear();
    return false;
  }

  // stack vs queue en termes de perfs ? stack: localité, queue: moins de doubles accès
  while (!ops_to_move.empty()) {
    // récupération des identifiants
    unsigned oid = ops_to_move.front();
    ops_to_move.pop_front();

    // mise-à-jour des valeurs de l'opération
    new_end_date = UpdateOperation(oid);

    // vérification que la nouvelle solution reste sub-critique
    if (new_end_date >= solution.Makespan()
        && ref_pb.nextOperation[oid] == ref_pb.FinalOp
        && draft_solution.ChildOnMachine(oid) == ref_pb.FinalOp) {
      ops_to_move.clear();
      return false;
    }
  }
  return true;
}

int LaarhovenSearch::SwapAndUpdateOps(unsigned parent, unsigned child)
{
  // inversion parent / successeur dans l'ordre sur les machines
  int end_date = draft_solution.SwapOperations(parent, child);

  // add successors of the swapped ops to be updated
  OperationId child_in_parent_job = ref_pb.nextOperation[parent];
  if (draft_solution.TryResetOperation(child_in_parent_job))
    ops_to_move.push_back(child_in_parent_job);

  OperationId child_in_child_job = ref_pb.nextOperation[child];
  if (draft_solution.TryResetOperation(child_in_child_job))
    ops_to_move.push_back(child_in_child_job);

  OperationId child_on_mac = draft_solution.ChildOnMachine(parent);
  if (draft_solution.TryResetOperation(child_on_mac)) {
    ops_to_move.push_back(child_on_mac);
  }

  return end_date;
}

int LaarhovenSearch::UpdateOperation(unsigned oid)
{
  int end_date = draft_solution.RescheduleOperation(oid);

  // add successors
  OperationId child_in_job = ref_pb.nextOperation[oid];
  if (draft_solution.TryResetOperation(child_in_job))
    ops_to_move.push_back(child_in_job);

  OperationId child_on_mac = draft_solution.ChildOnMachine(oid);
  if (draft_solution.TryResetOperation(child_on_mac))
    ops_to_move.push_back(child_on_mac);

  return end_date;
}
