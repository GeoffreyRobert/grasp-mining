#include "gtest/gtest.h"

#include <sstream>
#include <string>

#include "data/problem.h"
#include "data/solution.h"
#include "local-search/laarhoven_search.h"

using std::string;

class ProblemTest : public ::testing::Test {
protected:
    const string input =
        "6 6 55\n"
        "    2  1    0  3    1  6    3  7    5  3    4  6\n"
        "    1  8    2  5    4 10    5 10    0 10    3  4\n"
        "    2  5    3  4    5  8    0  9    1  1    4  7\n"
        "    1  5    0  5    2  5    3  3    4  8    5  9\n"
        "    2  9    1  3    4  5    5  4    0  3    3  1\n"
        "    1  3    3  3    5  9    0  7    4  4    2  1\n";
    const int size = 36;
    const int nJob = 6;
    const int nMac = 6;
    const int lowerBound = 55;

};

TEST_F(ProblemTest, StringStreamTest) {

    std::istringstream stream(input);
    Problem problem = LoadProblemFromStream(stream);

    EXPECT_EQ(problem.size, size);
    EXPECT_EQ(problem.nJob, nJob);
    EXPECT_EQ(problem.nMac, nMac);
    EXPECT_EQ(problem.lowerBound, lowerBound);
    ASSERT_EQ(problem.ToString(), input);
}

TEST_F(ProblemTest, FilePathTest) {
    const string file_path =
        "BINARY_DIR/instances/laar_test.txt";
    Problem problem = LoadProblemFromPath(file_path);

    EXPECT_EQ(problem.size, size);
    EXPECT_EQ(problem.nJob, nJob);
    EXPECT_EQ(problem.nMac, nMac);
    EXPECT_EQ(problem.lowerBound, lowerBound);
    ASSERT_EQ(problem.ToString(), input);
}
