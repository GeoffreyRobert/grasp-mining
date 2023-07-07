#include <algorithm>
#include <cmath>
#include <cassert>
#include <exception>

#include "pattern_miner.h"
#include "data/problem.h"
#include "data/solution.h"
#include "miner/transaction_encoder.h"
#include "data.h"
#include "fsout.h"
#include "fpmax.h"

PatternMiner::PatternMiner(const Problem& problem, double support, const TransactionEncoder& encoder)
  : DataMiner(problem)
  , _encoder(encoder)
  , _support(support)
{
}

std::vector<int> PatternMiner::SolutionToVec(const Solution& solution) const
{
  std::vector<int> t_vec(ref_pb.size);
  t_vec.clear();
  for (OperationId oid = ref_pb.OriginOp + 1; oid < ref_pb.FinalOp; ++oid)
  {
    OperationId prev_oid = solution.ParentOnMachine(oid);
    int itid = _encoder.OperationPairToItem(prev_oid, oid);

    // add the item to the transaction
    t_vec.push_back(itid);
  }
  return t_vec;
}

void PatternMiner::operator()(const vector<Solution>& solutions)
{
  // number of solutions to mine
  size_t t_num = solutions.size();

  // encode solutions as transactions to mine
  std::vector<Transaction> t_list(t_num);
  t_list.clear();
  for (const auto& solution : solutions)
  {
    t_list.emplace_back(SolutionToVec(solution));
  }
  VectorData transactions(std::move(t_list));

  int support = static_cast<int>(
      std::lround(_support * static_cast<double>(t_num)));

  VectorOut out_data;
  // maximal itemset mining
  int res = fpmax(transactions, support, &out_data);
  if (res != 0)
    throw std::runtime_error("fpmax algorithm did not complete nominally");

  // decode into itemsets of operation pairs
  std::vector<std::vector<std::pair<OperationId, OperationId>>> itemsets;
  for (const auto& i_vec : out_data.GetItemsets())
  {
    std::vector<std::pair<OperationId, OperationId>> itemset(i_vec.size());
    itemset.clear();
    for (auto itid : i_vec)
    {
      itemset.emplace_back(_encoder.ItemToOperationPair(itid));
    }
    itemsets.emplace_back(std::move(itemset));
  }
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
