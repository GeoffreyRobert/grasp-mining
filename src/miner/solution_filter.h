#ifndef SOLUTION_FILTER_
#define SOLUTION_FILTER_

#include <vector>

class Solution;

using std::vector;

class SolutionFilter
{
public:
  virtual ~SolutionFilter() {};
  virtual vector<Solution>& operator()(vector<Solution>& solutions) const = 0;
};

#endif // SOLUTION_FILTER_
