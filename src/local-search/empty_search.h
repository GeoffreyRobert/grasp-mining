#ifndef EMPTY_SEARCH_H_
#define EMPTY_SEARCH_H_

#include "local_search.h"

class EmptySearch : public LocalSearch {
public:
	Solution& operator()(Solution&) override;

protected:
  void Write(std::ostream&) const override;
};
#endif // !EMPTY_SEARCH_H_
