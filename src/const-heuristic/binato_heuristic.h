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
  BinatoHeuristic(
      const Problem&
    , CandidateGenerator<BinCandidateJob>& generator
    , double alpha = 0.5, unsigned seed = 0);

private:
  BinCandidateJob& CandidateSelection(vector<BinCandidateJob>&, Solution&) override;
  std::pair<int, int> CandidatesGeneration(vector<BinCandidateJob>&, Solution&);
  double _alpha = 0.5;

  // Restricted Candidate List
  vector<size_t> rc_list;
};

#endif // BINATO_HEURISTIC_H_
