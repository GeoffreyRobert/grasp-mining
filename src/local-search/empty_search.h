#ifndef EMPTY_SEARCH_H_
#define EMPTY_SEARCH_H_

#include "local_search.h"

class EmptySearch : public LocalSearch {
public:
	Solution& operator()(Solution&);
};
#endif // !EMPTY_SEARCH_H_
