#include "median_filter.h"

MedianFilter::MedianFilter(double threshold)
  : _threshold(threshold)
{
}

vector<Solution>& MedianFilter::operator()(vector<Solution>& solutions)
{
  return solutions;
}
