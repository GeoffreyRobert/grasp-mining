#include "empty_search.h"

Solution& EmptySearch::operator() (Solution& solution) {
	return solution;
}

void EmptySearch::Write(std::ostream& stream) const
{
  stream << "EmptySearch";
}
