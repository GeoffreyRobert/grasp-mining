#include "gtest/gtest.h"

#include "data/problem.h"
#include "data/solution.h"
#include "const-heuristic/const_heuristic.h"
#include "const-heuristic/hybrid_generator.h"
#include "miner/empty_miner.h"

namespace {

TEST(HybridGeneratorTest, Generation_Should_Let_Tied_Operation_Be_Scheduled_After_Loose_One)
{
  /* Operations in job 0 and 1 are tied in an itemset.
   * The operation in 1 will be scheduled at the very
   * end however.
   *
   *   <->
   * 0 0 12
   * 1 112
   */
  Problem problem(3, 2, 1,
    vector<std::pair<MachineId, int>> {
      { 1, 0 }, { 0, 1 },
      { 1, 2 }, { 0, 1 },
      { 1, 1 }, { 0, 1 },
    });
  Solution solution(problem);

  EmptyMiner data_miner(problem);
  data_miner.SetItemset({ { 2, 4 }, });
  HybridGenerator generator(problem, data_miner);

  generator.Init();
  auto& candidates0 = generator(solution);
  EXPECT_EQ(candidates0[0].jid, 0);
  EXPECT_EQ(candidates0[0].makespan, 0);
  EXPECT_EQ(candidates0[1].jid, 1);
  EXPECT_EQ(candidates0[1].makespan, 2);
  EXPECT_EQ(candidates0[2].jid, 2);
  EXPECT_EQ(candidates0[2].makespan, 1);

  solution.AddOperation(1);
  generator.IncrementJob(0);
  auto& candidates1 = generator(solution);
  EXPECT_EQ(candidates1[0].jid, 0);
  EXPECT_EQ(candidates1[0].makespan, 1);

  solution.AddOperation(3);
  generator.IncrementJob(1);
  auto& candidates2 = generator(solution);
  EXPECT_EQ(candidates2[1].jid, 1);
  EXPECT_EQ(candidates2[1].makespan, 3);

  solution.AddOperation(2);
  generator.IncrementJob(0);
  auto& candidates3 = generator(solution);
  EXPECT_EQ(candidates3[0].jid, 2);
  EXPECT_EQ(candidates3[0].makespan, 3);

  solution.AddOperation(5);
  generator.IncrementJob(0);
  auto& candidates4 = generator(solution);
  EXPECT_EQ(candidates4[0].jid, 2);
  EXPECT_EQ(candidates4[0].makespan, 4);

  solution.AddOperation(6);
  generator.IncrementJob(0);
  auto& candidates5 = generator(solution);
  EXPECT_EQ(candidates5[0].jid, 1);
  EXPECT_EQ(candidates5[0].makespan, 5);

  solution.AddOperation(4);
  generator.IncrementJob(0);
  auto& candidates6 = generator(solution);
  EXPECT_EQ(candidates6.size(), 0);
}

TEST(HybridGeneratorTest, Generation_Should_Squeeze_Suitable_Operations_Between_Tied_Operations)
{
  /* Operations in job 0 and 1 are tied in an itemset.
   * The operation in 2 is added after 0 has been scheduled
   * and 1 prioritized. It is squeezed between 0 and 1.
   *
   *   <->
   * 0 021
   * 1 112
   */
  Problem problem(3, 2, 1,
    vector<std::pair<MachineId, int>> {
      { 1, 0 }, { 0, 1 },
      { 1, 2 }, { 0, 1 },
      { 0, 1 }, { 1, 1 },
    });
  Solution solution(problem);

  EmptyMiner data_miner(problem);
  data_miner.SetItemset({ { 2, 4 }, });
  HybridGenerator generator(problem, data_miner);

  generator.Init();
  auto& candidates0 = generator(solution);
  EXPECT_EQ(candidates0[0].jid, 0);
  EXPECT_EQ(candidates0[0].makespan, 0);
  EXPECT_EQ(candidates0[1].jid, 1);
  EXPECT_EQ(candidates0[1].makespan, 2);
  EXPECT_EQ(candidates0[2].jid, 2);
  EXPECT_EQ(candidates0[2].makespan, 1);

  solution.AddOperation(1);
  generator.IncrementJob(0);
  auto& candidates1 = generator(solution);
  EXPECT_EQ(candidates1[0].jid, 0);
  EXPECT_EQ(candidates1[0].makespan, 1);

  solution.AddOperation(3);
  generator.IncrementJob(1);
  auto& candidates2 = generator(solution);
  EXPECT_EQ(candidates2[1].jid, 1);
  EXPECT_EQ(candidates2[1].makespan, 3);

  solution.AddOperation(2);
  generator.IncrementJob(0);
  auto& candidates3 = generator(solution);
  EXPECT_EQ(candidates3[0].jid, 2);
  EXPECT_EQ(candidates3[0].makespan, 2);
  EXPECT_EQ(candidates2[1].jid, 1);
  EXPECT_EQ(candidates2[1].makespan, 3);

  solution.AddOperation(5);
  generator.IncrementJob(0);
  auto& candidates4 = generator(solution);
  EXPECT_EQ(candidates4[0].jid, 2);
  EXPECT_EQ(candidates4[0].makespan, 3);

  solution.AddOperation(6);
  generator.IncrementJob(0);
  auto& candidates5 = generator(solution);
  EXPECT_EQ(candidates5[0].jid, 1);
  EXPECT_EQ(candidates5[0].makespan, 3);

  solution.AddOperation(4);
  generator.IncrementJob(0);
  auto& candidates6 = generator(solution);
  EXPECT_EQ(candidates6.size(), 0);
}

TEST(HybridGeneratorTest, Generation_Should_Penalize_Overlap_With_Tied_Operation)
{
  /* Operations in job 0 and 1 are tied in an itemset.
   * The operation in 2 is added after 0 has been scheduled
   * and 1 prioritized. It is penalized because it can't be
   * squeezed between 0 and 1.
   *
   *   <->
   * 0 0 122
   * 1 11
   */
  Problem problem(3, 2, 1,
    vector<std::pair<MachineId, int>> {
      { 1, 0 }, { 0, 1 },
      { 1, 2 }, { 0, 1 },
      { 1, 0 }, { 0, 2 },
    });
  Solution solution(problem);

  EmptyMiner data_miner(problem);
  data_miner.SetItemset({ { 2, 4 }, });
  HybridGenerator generator(problem, data_miner);

  generator.Init();
  auto& candidates0 = generator(solution);
  EXPECT_EQ(candidates0[0].jid, 0);
  EXPECT_EQ(candidates0[0].makespan, 0);
  EXPECT_EQ(candidates0[1].jid, 1);
  EXPECT_EQ(candidates0[1].makespan, 2);
  EXPECT_EQ(candidates0[2].jid, 2);
  EXPECT_EQ(candidates0[2].makespan, 0);

  solution.AddOperation(1);
  generator.IncrementJob(0);
  auto& candidates1 = generator(solution);
  EXPECT_EQ(candidates1[0].jid, 0);
  EXPECT_EQ(candidates1[0].makespan, 1);

  solution.AddOperation(5);
  generator.IncrementJob(2);
  auto& candidates2 = generator(solution);
  EXPECT_EQ(candidates2[2].jid, 2);
  EXPECT_EQ(candidates2[2].makespan, 2);

  solution.AddOperation(3);
  generator.IncrementJob(1);
  auto& candidates3 = generator(solution);
  EXPECT_EQ(candidates3[1].jid, 1);
  EXPECT_EQ(candidates3[1].makespan, 3);

  solution.AddOperation(2);
  generator.IncrementJob(0);
  auto& candidates4 = generator(solution);
  EXPECT_EQ(candidates4[0].jid, 2);
  EXPECT_EQ(candidates4[0].makespan, 5); // <- important

  // still scheduled tight
  solution.AddOperation(6);
  generator.IncrementJob(0);
  auto& candidates5 = generator(solution);
  EXPECT_EQ(candidates5[0].jid, 1);
  EXPECT_EQ(candidates5[0].makespan, 4);

  solution.AddOperation(4);
  generator.IncrementJob(0);
  auto& candidates6 = generator(solution);
  EXPECT_EQ(candidates6.size(), 0);
}

}
