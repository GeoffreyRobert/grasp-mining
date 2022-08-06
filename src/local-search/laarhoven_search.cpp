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
          ops_to_move.clear();
        }
      }
    }
  }
  return {updated_critical, updated_makespan};
}

void LaarhovenSearch::SwapAndUpdateOps(Solution& sol, unsigned parent, unsigned child)
{
  // inversion parent / successeur dans l'ordre sur les machines
  sol.SwapOperations(parent, child);

  // intialisation des dates pour le successeur
  // always update the predecessor before the successor
  UpdateOp(sol, child);
  UpdateOp(sol, parent);
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

  // add successors of the current op to be updated
  // ajout des successeurs à traiter
  if (sol.macChild[oid] != Problem::InvalidOp
      && is_changed[sol.macChild[oid]] % 2 != OpUpdate::ToChange) {
    ops_to_move.push_back(sol.macChild[oid]);
    ++is_changed[sol.macChild[oid]]; // Unchanged -> ToChange and Changed -> ChangedToChange
  }
  if (ref_pb.nextOperation[oid] != Problem::InvalidOp
      && is_changed[ref_pb.nextOperation[oid]] % 2 != OpUpdate::ToChange) {
    ops_to_move.push_back(ref_pb.nextOperation[oid]);
    ++is_changed[ref_pb.nextOperation[oid]];
  }
}

void LaarhovenSearch::CancelSwap(Solution& sol, unsigned parent, unsigned child)
{
  sol.SwapOperations(child, parent);
  std::fill(is_changed.begin(), is_changed.end(), OpUpdate::Unchanged);
}
