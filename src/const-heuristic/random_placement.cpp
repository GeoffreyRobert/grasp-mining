#include <numeric>

#include "random_placement.h"

using std::vector;

RandomPlacement::RandomPlacement(const Problem& problem)
  : ConstHeuristic(problem)
  , generator(std::random_device()())
{
}

Solution& RandomPlacement::operator()(Solution& solution)
{
  // Gestion des contraintes de d�pendance
  vector<JobId> num_ops_of_job(ref_pb.nJob, 0); // nombre d'op. trait�es par job
  vector<JobId> jobs_left(ref_pb.nJob); // jobs restant � ajouter � la sol.
  iota(jobs_left.begin(), jobs_left.end(), 0);

  for (OperationId counter = 0; counter < ref_pb.opNum; ++counter) {
    JobId jid = ChooseRandomJob(jobs_left); // generation d'un entier al�atoire

    // r�cup�ration des identifiants operation, machine et parent
    OperationId oid = ref_pb.operationNumber[jid][num_ops_of_job[jid]];

    // construction de la solution
    solution.GetOperationScheduling(oid);
    solution.AddOperation(oid);

    // stockage des d�pendances
    ++num_ops_of_job[jid];
    if (num_ops_of_job[jid] == ref_pb.nMac) {
      jobs_left[jid] = jobs_left.back(); // on recup�re le nombre de machines d'un job non termin�
      jobs_left.pop_back();
    }
  }
  return solution;
}

JobId RandomPlacement::ChooseRandomJob(const vector<JobId>& jobs_left)
{
  std::uniform_int_distribution<unsigned long> uni(0, jobs_left.size());
  return jobs_left[uni(generator)];
}
