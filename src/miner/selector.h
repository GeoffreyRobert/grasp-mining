#ifndef SELECTOR_
#define SELECTOR_

#include <vector>

class Solution;

using std::vector;

class Selector
{
public:
	virtual ~Selector() {};
	virtual vector<Solution>& operator()(vector<Solution>& solutions);
};

#endif // SELECTOR_
