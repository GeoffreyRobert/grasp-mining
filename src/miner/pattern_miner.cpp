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

vector<Transaction> PatternMiner::SolutionsToVectors(
    const vector<Solution>& solutions) const
{
  size_t t_num = solutions.size();

  // encode solutions as transactions to mine
  vector<Transaction> t_list;
  t_list.reserve(t_num);
  for (auto& solution : solutions)
  {
    vector<int> t_vec(ref_pb.size);
    t_vec.clear();
    for (OperationId oid = ref_pb.OriginOp + 1; oid < ref_pb.FinalOp; ++oid)
    {
      OperationId prev_oid = solution.ParentOnMachine(oid);
      int itid = _encoder.OperationPairToItem(prev_oid, oid);

      // add the item to the transaction
      t_vec.push_back(itid);
    }
    t_list.emplace_back(std::move(t_vec));
  }

  return t_list;
}

void PatternMiner::operator()(const vector<Solution>& solutions)
{
  // number of solutions to mine
  size_t t_num = solutions.size();

  int support = static_cast<int>(
      std::lround(_support * static_cast<double>(t_num)));
  unsigned num_itemsets = static_cast<unsigned>(
      std::lround(_itemset_ratio * static_cast<double>(t_num)));
  unsigned num_range = static_cast<unsigned>(
      std::lround(0.5 * static_cast<double>(num_itemsets)));

  // maximal itemset mining
  auto init = high_resolution_clock::now();
  VectorData transactions(SolutionsToVectors(solutions));
  vector<vector<int>> raw_itemsets;
  while (raw_itemsets.size() < num_itemsets)
  {
    if (support <= 0)
      throw std::runtime_error("fpmax couldn't find any recurring pattern");
    VectorOut out_data;
    int res = fpmax(transactions, support, &out_data);
    if (transactions.getNextTransaction() != nullptr)
      throw std::runtime_error("fpmax did not scan DB properly");
    if (res != 0)
      throw std::runtime_error("fpmax algorithm did not complete nominally");
    raw_itemsets = out_data.GetItemsets();

    // adapt support depending on current number of itemsets mined
    if (num_itemsets - raw_itemsets.size() < num_range)
    {
      support = static_cast<int>(
          std::lround((1.0 + _support_mult) / 2.0 * static_cast<double>(support)));
    }
    else
    {
      support = static_cast<int>(
          std::lround(_support_mult * static_cast<double>(support)));
    }
  }

  // decode into itemsets of operation pairs
  _itemsets.clear();
  _itemsets.reserve(raw_itemsets.size());
  for (const auto& i_vec : raw_itemsets)
  {
    std::vector<std::pair<OperationId, OperationId>> itemset(i_vec.size());
    itemset.clear();
    for (auto itid : i_vec)
    {
      itemset.emplace_back(_encoder.ItemToOperationPair(itid));
    }
    _itemsets.emplace_back(std::move(itemset));
  }

  // sort patterns by decreasing size
  std::sort(_itemsets.begin(), _itemsets.end()
      , [](const auto& a, const auto& b)
      {
        return b.size() < a.size(); // non-increasing ordering
      });

  // truncate vector to target size
  if (num_itemsets < raw_itemsets.size())
    raw_itemsets.resize(num_itemsets);
  _iter = _itemsets.begin();

  runtime += duration_cast<microseconds>(high_resolution_clock::now() - init);
}

const vector<pair<OperationId, OperationId>>& PatternMiner::GetItemset()
{
  auto& res = *(_iter++);
  if (_iter == _itemsets.end())
    _iter = _itemsets.begin();
  return res;
}
