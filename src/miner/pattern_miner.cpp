#include <algorithm>
#include <cmath>
#include <cassert>
#include <exception>

#include "pattern_miner.h"
#include "data/problem.h"
#include "data/solution.h"
#include "miner/transaction_encoder.h"
#include "miner/solution_filter.h"
#include "data.h"
#include "fsout.h"
#include "fpmax.h"

PatternMiner::PatternMiner(
    const Problem& problem
  , const TransactionEncoder& encoder
  , const SolutionFilter& filter
  , double support)
  : DataMiner(problem)
  , _encoder(encoder)
  , _filter(filter)
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

void PatternMiner::operator()(vector<Solution>& solution_set)
{
  auto& filtered_solutions = _filter(solution_set);

  // number of solutions to mine
  size_t t_num = filtered_solutions.size();

  int support = static_cast<int>(
      std::lround(_support * static_cast<double>(t_num)));
  unsigned num_itemsets = static_cast<unsigned>(
      std::lround(_itemset_ratio * static_cast<double>(t_num)));
  unsigned num_range = static_cast<unsigned>(
      std::lround(0.5 * static_cast<double>(num_itemsets)));

  // maximal itemset mining
  auto init = high_resolution_clock::now();
  VectorData transactions(SolutionsToVectors(filtered_solutions));
  vector<pair<int, vector<int>>> raw_itemsets;
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

  // get the itemsets with highest support
  auto middle = begin(raw_itemsets) + num_itemsets;
  std::partial_sort(begin(raw_itemsets), middle, end(raw_itemsets)
      , [](const auto& a, const auto& b)
      {
        return a.first > b.first; // non-increasing ordering [ ..., a, b, ... ]
      });

  // decode into itemsets of operation pairs
  _itemsets.clear();
  _itemsets.reserve(num_itemsets);
  for (const auto& i_pair : raw_itemsets)
  {
    auto& i_vec = i_pair.second;
    vector<pair<OperationId, OperationId>> itemset;
    itemset.reserve(i_vec.size());
    for (auto itid : i_vec)
    {
      itemset.emplace_back(_encoder.ItemToOperationPair(itid));
    }
    _itemsets.emplace_back(std::move(itemset));
  }
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
