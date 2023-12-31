#include "gtest/gtest.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "data/problem.h"
#include "data/solution.h"
#include "local-search/laarhoven_search.h"

using std::string;

namespace {

TEST(ProblemTest, StringStreamTest) {
    const string input =
        " 6 6 55\n"
        " 2  1  0  3  1  6  3  7  5  3  4  6\n"
        " 1  8  2  5  4 10  5 10  0 10  3  4\n"
        " 2  5  3  4  5  8  0  9  1  1  4  7\n"
        " 1  5  0  5  2  5  3  3  4  8  5  9\n"
        " 2  9  1  3  4  5  5  4  0  3  3  1\n"
        " 1  3  3  3  5  9  0  7  4  4  2  1\n";
    const int opNum = 36;
    const int size = opNum + 2;
    const int nJob = 6;
    const int nMac = 6;
    const int lowerBound = 55;

    std::istringstream stream(input);
    Problem problem = LoadProblemFromStream(stream);

    EXPECT_EQ(problem.opNum, opNum);
    EXPECT_EQ(problem.size, size);
    EXPECT_EQ(problem.nJob, nJob);
    EXPECT_EQ(problem.nMac, nMac);
    EXPECT_EQ(problem.lowerBound, lowerBound);
    ASSERT_EQ(problem.ToString(), input);
}

TEST(ProblemTest, FilePathTest) {
    const string file_path =
        string(BINARY_DIR) + "/instances/laar_test.txt";
    std::stringstream stream_input;
    stream_input << std::ifstream(file_path, std::ios::in).rdbuf();

    auto problem = LoadProblemFromStream(stream_input);

    ASSERT_EQ(problem.ToString(), stream_input.str());
}

} // namespace
