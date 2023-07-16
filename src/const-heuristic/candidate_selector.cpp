#include "const-heuristic/candidate_selector.h"

CandidateSelector::CandidateSelector(const Problem& problem, unsigned seed)
  : SolverModule(problem)
  , _r_generator(seed)
{}

std::ostream& operator<<(std::ostream& stream, const CandidateSelector& selector)
{
  selector.Write(stream);
  return stream;
}
