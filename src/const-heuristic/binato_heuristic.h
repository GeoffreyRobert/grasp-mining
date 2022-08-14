#ifndef BINATO_HEURISTIC_H_
#define BINATO_HEURISTIC_H_

#include <random>
#include <vector>

#include "const_heuristic.h"

class BinatoHeuristic : public ConstHeuristic {
public:
  BinatoHeuristic(const Problem&, double alpha = 0.5);

  // construction greedy randomis�e
  Solution& operator()(Solution&);

private:
  double _alpha = 0.5;
  std::mt19937 generator;

  // Gestion des contraintes de d�pendance
  vector<MachineId> num_ops_of_job; // nombre d'op. trait�es par job

  // Gestion de la Restricted Candidate List
  vector<JobId> rc_list; // Restricted Candidate List

  // Gestion des candidats � la RCL, de leur parent et du makespan
  vector<JobId> candidate_jobs;
  vector<int> tmp_mkspan_list;
};
#endif // !BINATO_HEURISTIC_H_
