#ifndef MEDIAN_FILTER_
#define MEDIAN_FILTER_

#include "solution_filter.h"

class MedianFilter : public SolutionFilter
{
public:
  MedianFilter(double threshold);
	vector<Solution>& operator()(vector<Solution>& solutions) override;

private:
  const double _threshold;
};

#endif // MEDIAN_FILTER_
