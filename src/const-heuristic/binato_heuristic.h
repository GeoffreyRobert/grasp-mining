#ifndef BINATO_HEURISTIC_H_
#define BINATO_HEURISTIC_H_

#include <random>
#include <vector>

#include "const-heuristic/const_heuristic.h"

struct BinCandidateJob : public CandidateJob {
  int makespan = 0;
};

class BinatoHeuristic : public CandidateHeuristic<BinCandidateJob> {
public:
  BinatoHeuristic(const Problem&, double alpha = 0.5, unsigned seed = 0);

  // construction greedy randomisée
  Solution& operator()(Solution&) override;

private:
  void virtual Init() override;
  double _alpha = 0.5;

  // Restricted Candidate List
  vector<BinCandidateJob*> rc_list;
};

#endif // !BINATO_HEURISTIC_H_
