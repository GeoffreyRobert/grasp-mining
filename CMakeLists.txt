cmake_minimum_required(VERSION 3.12)

project(grasp-mining 
	VERSION 0.0
	LANGUAGES CXX
)

add_executable(grasp
	src/main.cpp
	src/const-heuristic/binato_heuristic.cpp
	src/const-heuristic/random_placement.cpp
	src/data/problem.cpp
	src/data/solution.cpp
    src/data/solution_impl.cpp
	src/local-search/empty_search.cpp
	src/local-search/laarhoven_search.cpp
	src/miner/empty_miner.cpp
	src/miner/pattern_miner.cpp
	src/solver/solver.cpp
	#src/solver/solver_factory.cpp
	#src/util/Out_JS.cpp
)

target_compile_features(grasp
	PRIVATE cxx_std_11
)

target_include_directories(grasp
	PUBLIC src/
)

add_executable(laarhoven_test
	src/tests/laarhoven_test.cpp
	src/data/problem.cpp
	src/data/solution.cpp
	src/local-search/laarhoven_search.cpp
)

target_compile_features(laarhoven_test
	PRIVATE cxx_std_11
)

target_include_directories(laarhoven_test
	PUBLIC src/
)