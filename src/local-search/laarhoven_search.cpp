#include <deque>
#include <utility>

#include "laarhoven_search.h"

using std::deque;

LaarhovenSearch::LaarhovenSearch(const Problem& problem)
  : LocalSearch(problem)
{
  is_changed.resize(ref_pb.size, OpUpdate::Unchanged);
  new_start_date.resize(ref_pb.size);
  new_end_date.resize(ref_pb.size);
  new_is_crit_mac.resize(ref_pb.size);
  ops_to_move.set_capacity(ref_pb.size);
}

Solution& LaarhovenSearch::operator()(Solution& sol)
{
  OperationId operation = sol.criticalOp; // op. considérée pour relocation
  OperationId parent = sol.macParent[operation]; // parent de l'operation

  // remonter le chemin critique
  do {
    if (!sol.isCritMachine[operation]) {
      // on ignore deux ops. dans le meme job
      operation = ref_pb.prevOperation[operation];
    } else {
      auto [updated_critical, updated_makespan] = SwapAndEvaluate(sol, parent, operation);
      // on inverse deux ops. sur le chemin critique si réduction du makespan
      if (updated_critical != Problem::InvalidOp && updated_makespan < sol.makespan) {
        operation = sol.DoChanges(is_changed, new_start_date, new_end_date, new_is_crit_mac);
        std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
        hit_count++; // TODO: sous preproc. pour debug
      } else {
        // on continue sur le chemin critique
        operation = parent;
      }
    }
    parent = sol.macParent[operation];
  } while (parent != Problem::InvalidOp);
  return sol;
}

/** En charge de la sélection des opérations à examiner
 *  Returns the id of the operation that was updated and is scheduled the latest
 **/
std::pair<OperationId, int> LaarhovenSearch::SwapAndEvaluate(Solution& sol, OperationId parent, OperationId child)
{
  // cas qui allonge forcément le chemin critique
  if (ref_pb.prevOperation[child] != Problem::InvalidOp && sol.startDate[parent] < sol.endDate[ref_pb.prevOperation[child]]) {
    return {Problem::InvalidOp, sol.makespan};
  }

  // inversion des 2 opérations sur le chemin critique
  SwapAndUpdateOps(sol, parent, child);

  // initialisation de l'op. crit. et du makespan
  unsigned updated_critical = parent; // "rightmost" operation among all updated
  int updated_makespan = new_end_date[parent];

  // successeurs des operation swappees a traiter
  OperationId next_from_parent = ref_pb.nextOperation[parent];
  if (next_from_parent != Problem::InvalidOp) {
    ops_to_move.push_back(next_from_parent);
    is_changed[next_from_parent] = OpUpdate::ToChange;
  }
  OperationId next_from_child = ref_pb.nextOperation[child];
  if (next_from_child != Problem::InvalidOp) {
    ops_to_move.push_back(next_from_child);
    is_changed[next_from_child] = OpUpdate::ToChange;
  }
  // operation suivante sur la machine des operations swappees
  OperationId next_on_machine = sol.macChild[parent];
  if (next_on_machine != Problem::InvalidOp) {
    ops_to_move.push_back(next_on_machine);
    is_changed[next_on_machine] = OpUpdate::ToChange;
  }

  unsigned oid;
  // stack vs queue en termes de perfs ? stack: localité, queue: moins de doubles accès
  while (!ops_to_move.empty()) {
    // récupération des identifiants
    oid = ops_to_move.front();
    ops_to_move.pop_front();

    // mise-à-jour des valeurs de l'opération
    UpdateOp(sol, oid);

    // vérification que la nouvelle solution reste sub-critique
    if (ref_pb.nextOperation[oid] == Problem::InvalidOp && sol.macChild[oid] == Problem::InvalidOp) {
      if (new_end_date[oid] >= updated_makespan) {
        updated_critical = oid;
        updated_makespan = new_end_date[oid];
        if (updated_makespan >= sol.makespan) {
          CancelSwap(sol, parent, child);
          break;
        }
      }
    }
    AddSuccessors(sol, oid);
  }
  return {updated_critical, updated_makespan};
}

void LaarhovenSearch::SwapAndUpdateOps(Solution& sol, unsigned parent, unsigned child)
{
  // intialisation des dates pour le successeur
  int date_job;
  if (ref_pb.prevOperation[child] != Problem::InvalidOp) {
    date_job = sol.endDate[ref_pb.prevOperation[child]];
  } else {
    date_job = 0;
  }
  int date_mac;
  if (sol.macParent[parent] != Problem::InvalidOp) {
    date_mac = sol.endDate[sol.macParent[parent]];
  } else {
    date_mac = 0;
  }
  int date_par;
  if (date_job < date_mac) {
    date_par = date_mac;
    new_is_crit_mac[child] = true;
  } else {
    date_par = date_job;
    new_is_crit_mac[child] = false;
  }
  new_start_date[child] = date_par;
  new_end_date[child] = date_par + ref_pb.timeOnMachine[child];
  is_changed[child] = OpUpdate::Changed;

  // intialisation des dates pour le parent
  if (ref_pb.prevOperation[parent] != Problem::InvalidOp) {
    date_job = sol.endDate[ref_pb.prevOperation[parent]];
  } else {
    date_job = 0;
  }
  date_mac = new_end_date[child];
  if (date_job < date_mac) {
    date_par = date_mac;
    new_is_crit_mac[parent] = true;
  } else {
    date_par = date_job;
    new_is_crit_mac[parent] = false;
  }
  new_start_date[parent] = date_par;
  new_end_date[parent] = date_par + ref_pb.timeOnMachine[parent];
  is_changed[parent] = OpUpdate::Changed;

  // inversion parent / successeur dans l'ordre sur les machines
  sol.SwapOperations(parent, child);
}

void LaarhovenSearch::CancelSwap(Solution& sol, unsigned parent, unsigned child)
{
  sol.SwapOperations(child, parent);
  std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
}

void LaarhovenSearch::UpdateOp(const Solution& sol, unsigned oid)
{
  // récupération des nouvelles dates parent / parent disj
  int date_mac;
  if (sol.macParent[oid] == Problem::InvalidOp) {
    date_mac = 0;
  } else if (is_changed[sol.macParent[oid]] >= OpUpdate::Changed) {
    date_mac = new_end_date[sol.macParent[oid]];
  } else {
    date_mac = sol.endDate[sol.macParent[oid]];
  }

  int date_job;
  if (ref_pb.prevOperation[oid] == Problem::InvalidOp) {
    date_job = 0;
  } else if (is_changed[ref_pb.prevOperation[oid]] >= OpUpdate::Changed) {
    date_job = new_end_date[ref_pb.prevOperation[oid]];
  } else {
    date_job = sol.endDate[ref_pb.prevOperation[oid]];
  }

  // date critique
  int date_par;
  if (date_job < date_mac) {
    date_par = date_mac;
    new_is_crit_mac[oid] = true;
  } else {
    date_par = date_job;
    new_is_crit_mac[oid] = false;
  }

  // nouvelles valeurs
  new_start_date[oid] = date_par;
  new_end_date[oid] = date_par + ref_pb.timeOnMachine[oid];
  is_changed[oid] = OpUpdate::Changed;
}

void LaarhovenSearch::AddSuccessors(const Solution& sol, unsigned oid)
{
  // ajout des successeurs à traiter
  if (sol.macChild[oid] != Problem::InvalidOp) {
    if (is_changed[sol.macChild[oid]] % 2 != OpUpdate::ToChange) {
      ops_to_move.push_back(sol.macChild[oid]);
      ++is_changed[sol.macChild[oid]]; // Unchanged -> ToChange and Changed -> ChangedToChange
    }
  }
  if (ref_pb.nextOperation[oid] != Problem::InvalidOp) {
    if (is_changed[ref_pb.nextOperation[oid]] % 2 != OpUpdate::ToChange) {
      ops_to_move.push_back(ref_pb.nextOperation[oid]);
      ++is_changed[ref_pb.nextOperation[oid]];
    }
  }
}
