#include "pattern_miner.h"

void PatternMiner::PatternNode::GetSuccessor(vector<int>& full_pattern, double support, int depth) {
	vector<int> cand_list(successors.size());
	for (pair<PatternNode, double> s : successors) {
		if (s.second > support) {
			cand_list.push_back(s.first.opNumber);
		}
	}

}

