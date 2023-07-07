#include <algorithm>
#include <cmath>

#include "median_filter.h"
#include "data/solution.h"

MedianFilter::MedianFilter(double threshold)
  : _threshold(threshold)
{
}

vector<Solution>& MedianFilter::operator()(vector<Solution>& solutions)
{
  auto middle = begin(solutions)
    + std::lround(_threshold * static_cast<double>(solutions.size()));

  std::partial_sort(begin(solutions), middle, end(solutions), std::greater());
  solutions.erase(middle, end(solutions));
  return solutions;
}
