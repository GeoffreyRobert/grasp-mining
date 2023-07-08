#ifndef HYBRID_HEURISTIC_H_
#define HYBRID_HEURISTIC_H_

#include <random>
#include <vector>

#include "const-heuristic/binato_heuristic.h"

class HybridHeuristic : public CandidateHeuristic<BinCandidateJob> {
public:
  HybridHeuristic(const Problem&, double alpha = 0.5, unsigned seed = 0);

private:
  BinCandidateJob& CandidateSelection(vector<BinCandidateJob>&, Solution&) override;
  std::pair<int, int> CandidatesGeneration(vector<BinCandidateJob>&, Solution&);
  double _alpha = 0.5;

  // Restricted Candidate List
  vector<size_t> rc_list;
};

#endif // HYBRID_HEURISTIC_H_
