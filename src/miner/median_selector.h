#ifndef MEDIAN_SELECTOR_
#define MEDIAN_SELECTOR_

#include "selector.h"

class MedianSelector : public Selector
{
public:
  MedianSelector(double threshold);
	vector<Solution>& operator()(vector<Solution>& solutions) override;

private:
  const double _threshold;
};

#endif // MEDIAN_SELECTOR_
