#include <cmath>

#include "pattern_miner.h"
#include "data/problem.h"

PatternMiner::PatternMiner(const Problem& problem, double support)
  : DataMiner(problem)
  , _support(support)
{
}

void PatternMiner::operator()(const vector<Solution>& solutions)
{
  (void)solutions;
}

void PatternMiner::PatternNode::GetSuccessor(vector<int>& full_pattern, double support, int depth) {
  (void)full_pattern;
  (void)depth;
	vector<int> cand_list(successors.size());
	for (pair<PatternNode, double> s : successors) {
		if (s.second > support) {
			cand_list.push_back(s.first.opNumber);
		}
	}

}

vector<int> PatternMiner::ProposePattern(OperationId operation, double support) {
  (void)operation;
  (void)support;
	return vector<int>();
}
