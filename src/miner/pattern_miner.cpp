#include <algorithm>
#include <cmath>
#include <cassert>

#include "pattern_miner.h"
#include "data/problem.h"
#include "data/solution.h"
#include "data.h"
#include "fsout.h"
#include "fpmax.h"

PatternMiner::PatternMiner(const Problem& problem, double support)
  : DataMiner(problem)
  , _support(support)
{
}

bool HasDuplicates(std::vector<int>& t_vec)
{
  std::sort(t_vec.begin(), t_vec.end());
  return std::adjacent_find(t_vec.begin(), t_vec.end()) != t_vec.end();
}

void PatternMiner::operator()(const vector<Solution>& solutions)
{
  // number of solutions to mine
  size_t t_num = solutions.size();

  // encode solutions as transactions to mine
  std::vector<Transaction> t_list(t_num);
  t_list.clear();
  for (auto& solution : solutions)
  {
    std::vector<int> t_vec(ref_pb.size);
    t_vec.clear();
    for (OperationId oid = ref_pb.OriginOp + 1; oid < ref_pb.FinalOp; ++oid)
    {
      OperationId prev_oid = solution.ParentOnMachine(oid);
      // encode OriginOp in JobId 0 and other ops in JobId+1
      JobId prev_jid = (prev_oid + ref_pb.nMac - 1) / ref_pb.nMac;

      // create a unique item number for a pair of successive operations
      // each operation can have a predecessor from the nJob or the origin job
      int itid = static_cast<int>(oid * (ref_pb.nJob + 1) + prev_jid);

      // add the item to the transaction
      assert(itid != t_vec.back() && "same item inserted twice");
      t_vec.push_back(itid);
    }
    assert(!HasDuplicates(t_vec) && "transaction contains duplicates");
    t_list.emplace_back(std::move(t_vec));
  }

  int support = static_cast<int>(
      std::lround(_support * static_cast<double>(t_num)));

  VectorData transactions(std::move(t_list));
  VectorOut out_data;
  // maximal itemset mining
  int res = fpmax(transactions, support, &out_data);
  assert(res == 0 && "fpmax algorithm didn't complete properly");

  std::vector<std::vector<int>> itemsets = out_data.GetItemsets();
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
