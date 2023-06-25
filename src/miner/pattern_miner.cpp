#include <cmath>

#include "pattern_miner.h"
#include "data/problem.h"
#include "data/solution.h"
extern "C" {
  #include "lcm_max.h"
}

PatternMiner::PatternMiner(const Problem& problem, double support)
  : DataMiner(problem)
  , _support(support)
{
}

void PatternMiner::operator()(const vector<Solution>& solutions)
{
  // encode solutions as transactions to mine
  auto max_num_t = solutions.size() * ref_pb.size;
  std::vector<int> transactions(max_num_t);
  transactions.clear();
  for (auto& solution : solutions)
  {
    for (OperationId oid = ref_pb.OriginOp + 1; oid < ref_pb.FinalOp; ++oid)
    {
      OperationId prev_oid = solution.ParentOnMachine(oid);
      // encode OriginOp in JobId 0 and other ops in JobId+1
      JobId prev_jid = (prev_oid + ref_pb.nMac - 1) / ref_pb.nMac;

      // create a unique item number for a pair of successive operations
      unsigned itid = oid * ref_pb.nJob + prev_jid;

      // add the item to the transaction
      transactions.push_back(static_cast<int>(itid));
    }
    transactions.push_back(-1);
  }
  transactions.push_back(-1);

  int support = static_cast<int>(
      std::lround(_support * static_cast<double>(solutions.size())));

  // maximal itemset mining
  int* store = LCMmax ( transactions.data(), support );
  LCMfree(store);
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
