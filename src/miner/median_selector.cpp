#include "median_selector.h"

MedianSelector::MedianSelector(double threshold)
  : _threshold(threshold)
{
}

vector<Solution>& MedianSelector::operator()(vector<Solution>& solutions)
{
  return solutions;
}
