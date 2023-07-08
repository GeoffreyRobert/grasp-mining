#include "const-heuristic/candidate_selector.h"

CandidateSelector::CandidateSelector(const Problem& problem, unsigned seed)
  : SolverModule(problem)
  , _r_generator(seed)
{}
