#include "local-search/local_search.h"

std::ostream& operator<<(std::ostream& stream, const LocalSearch& local_search)
{
  local_search.Write(stream);
  return stream;
}
