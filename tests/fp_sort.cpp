#include "gtest/gtest.h"

#include "sort.h"

namespace {

const int result[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

TEST(FpgrowthTest, Sort_Already_Sorted)
{
  int array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  int temp[] = {0, 1, 2, 3, 4, 5, 6, 7, 8 ,9};
  sort(array, temp, 0, 9);

  for (size_t i = 0; i < 10; ++i)
  {
    EXPECT_EQ(array[i], result[i]);
  }
}

TEST(FpgrowthTest, Sort_Inversely_Sorted)
{
  int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int temp[10];
  sort(array, temp, 0, 9);

  for (size_t i = 0; i < 10; ++i)
  {
    EXPECT_EQ(array[i], result[i]);
  }
}

TEST(FpgrowthTest, Sort_Back_And_Forth)
{
  int array[] = {5, 4, 6, 3, 7, 2, 8, 1, 9, 0};
  int temp[] = {4, 5, 3, 6, 2, 7, 1, 8, 0, 9};
  sort(array, temp, 0, 9);

  for (size_t i = 0; i < 10; ++i)
  {
    EXPECT_EQ(array[i], result[i]);
  }
}

TEST(FpgrowthTest, Sort_Out_To_In)
{
  int array[] = {0, 2, 4, 6, 8, 9, 7, 5, 3, 1};
  int temp[] = {9, 5, 3, 6, 2, 7, 1, 8, 0, 9};
  sort(array, temp, 0, 9);

  for (size_t i = 0; i < 10; ++i)
  {
    EXPECT_EQ(array[i], result[i]);
  }
}

}
