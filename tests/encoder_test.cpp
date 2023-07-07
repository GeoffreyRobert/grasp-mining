#include "gtest/gtest.h"

#include "data/problem.h"
#include "miner/transaction_encoder.h"

namespace {

TEST(EncoderTest, Encoding_Decoding_Should_Compose_To_Identity)
{
  Problem problem(6, 6, 55,
    std::vector<std::pair<MachineId, int>> {
      { 2, 1 }, { 0, 3 }, { 1, 6 }, { 3, 7 }, { 5, 3 }, { 4, 6 },
      { 1, 8 }, { 2, 5 }, { 4, 10 }, { 5, 10 }, { 0, 10 }, { 3, 4 },
      { 2, 5 }, { 3, 4 }, { 5, 8 }, { 0, 9 }, { 1, 1 }, { 4, 7 },
      { 1, 5 }, { 0, 5 }, { 2, 5 }, { 3, 3 }, { 4, 8 }, { 5, 9 },
      { 2, 9 }, { 1, 3 }, { 4, 5 }, { 5, 4 }, { 0, 3 }, { 3, 1 },
      { 1, 3 }, { 3, 3 }, { 5, 9 }, { 0, 7 }, { 4, 4 }, { 2, 1 },
    });

  TransactionEncoder encoder(problem);

  const OperationId prev_oid = 2;
  const OperationId oid = 20;
  int item = encoder.OperationPairToItem(prev_oid, oid);
  auto [r_prev_oid, r_oid] = encoder.ItemToOperationPair(item);

  EXPECT_EQ(prev_oid, r_prev_oid);
  EXPECT_EQ(oid, r_oid);
}

}
