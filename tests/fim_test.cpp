#include <iostream>
#include <vector>
#include "gtest/gtest.h"

extern "C" {
  #include "lcm_max.h"
}

namespace {

TEST(LcmMaxTest, SmokeTest)
{
  int D = -1;
  int buf[] = {
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    1, 2, 3, 4, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    5, 6, 7, 8, D,
    8, 9, D,
    8, 9, D,
    8, 9, D,
    8, 9, D,
    D,
  };

  const int th = 10;
  int* store = LCMmax ( buf, th );
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

}