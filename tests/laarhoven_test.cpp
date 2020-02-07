#include "gtest/gtest.h"

#include <vector>
#include <iostream>
#include <string>

#include "data/problem.h"
#include "data/solution.h"
#include "local-search/laarhoven_search.h"

using std::vector; using std::string;

TEST(LaarhovenTest, CompleteTest) {
	const string file_path = "C:\\Users\\geoff\\EMSE\\Projets Ecole\\"
		"Projet Recherche\\implementation\\grasp-mining\\instances\\laar_test.txt";

	Problem problem;
	problem.LoadProblemFromFile(file_path);

	// construction d'une solution optimisable
	Solution solution(problem);
	solution.startDate = 
	{  0, 1, 4,10,17,20,
	  10,18,26,36,46,56,
	   1, 6,20,28,37,38,
	  18,23,28,33,45,53,
	   6,23,53,62,66,69,
	   0, 3, 6,15,58,62
	};
	solution.endDate = 
	{  1, 4,10,17,20,26,
	  18,23,36,46,56,60,
	   6,10,28,37,38,45,
	  23,28,33,36,53,62,
	  15,26,58,66,69,70,
	   3, 6,15,22,62,63
	};
	solution.macParent =
	{ -1,-1,30,13,32,-1,
	   2,24, 5,14,15,21,
	   0, 6, 4,19,25, 8,
	   6,33, 7, 3,17, 9,
	  12,18,22,23,10,11,
	  -1,-1,-1, 1,26,20
	};
	solution.macChild =
	{ 12,33, 6,21,14, 8,
	  18,20,17,23,28,29,
	  24, 3, 9,10,-1,22,
	  25,15,35,11,26,27,
	   7,16,34,-1,-1,-1,
	   2,13, 4,19,-1,-1
	};
	solution.isCritMachine = 
	{  0, 0, 0, 0, 0, 0,
	   1, 0, 1, 0, 0, 0,
	   1, 0, 1, 0, 0, 0,
	   1, 0, 0, 0, 1, 0,
	   1, 1, 1, 1, 0, 0,
	   0, 0, 0, 0, 1, 0
	};
	solution.criticalOp = 29;
	solution.makespan = 70;

	LaarhovenSearch local_search;
    local_search.ResourcesAlloc(problem);
	local_search(problem, solution);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}