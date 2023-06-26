#include <iostream>
#include <vector>
#include "gtest/gtest.h"

extern "C" {
  #include "lcm_max.h"
}
#include "fpmax.h"
#include "data.h"
#include "fsout.h"

namespace {

std::vector<std::vector<int>> t_list = {
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 5, 6, 7, 8 },
  { 8, 9 },
  { 8, 9 },
  { 8, 9 },
  { 8, 9 },
};

TEST(FimTest, LcmMax)
{
  int D = -1;

  std::vector<int> buf;
  for (auto& transaction : t_list)
  {
    for (auto item : transaction)
      buf.push_back(item);
    buf.push_back(D);
  }
  buf.push_back(D);

  const int th = 10;
  int* store = LCMmax ( buf.data(), th );

  int* iter = store;
  std::vector<std::vector<int>> itemsetList;
  while (*iter != D)
  {
    auto& itemset = itemsetList.emplace_back();
    while (*iter != D)
    {
      itemset.push_back(*iter);
      ++iter;
    }
    ++iter;
  }
  LCMfree(store);

  EXPECT_EQ(itemsetList.size(), 2);
}

TEST(FimTest, fpgrowth)
{
  std::vector<Transaction> buf;
  for (auto&& t_vec : t_list)
  {
    buf.emplace_back(std::move(t_vec));
  }

  VectorData vec_data(std::move(buf));
  const int th = 10;
  VectorOut vec_out;
  int res = fpmax ( vec_data, th, &vec_out );

  EXPECT_EQ(res, 0);
  EXPECT_EQ(vec_out.GetItemsets().size(), 2);
}

}
