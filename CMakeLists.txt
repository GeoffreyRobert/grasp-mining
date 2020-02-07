cmake_minimum_required(VERSION 3.12)

project(grasp-mining
	VERSION 0.0
	LANGUAGES CXX
)

find_package(Boost)
find_package(GTest REQUIRED)
enable_testing()

add_executable(grasp
	src/main.cpp
	src/const-heuristic/binato_heuristic.cpp
	src/const-heuristic/random_placement.cpp
	src/data/problem.cpp
	src/data/solution.cpp
	src/local-search/empty_search.cpp
	src/local-search/laarhoven_search.cpp
	src/miner/empty_miner.cpp
	src/miner/pattern_miner.cpp
	src/solver/solver.cpp
)

target_compile_features(grasp
	PRIVATE cxx_std_11
)

target_include_directories(grasp
	PUBLIC src/
    PUBLIC ${Boost_INCLUDE_DIRS}
)


add_executable(laarhoven_test
	tests/laarhoven_test.cpp
	src/data/problem.cpp
	src/data/solution.cpp
	src/local-search/laarhoven_search.cpp
)

target_include_directories(laarhoven_test
	PUBLIC src/
    PUBLIC ${Boost_INCLUDE_DIRS}
)

target_link_libraries(laarhoven_test GTest::GTest GTest::Main)
add_test(NAME example_test COMMAND laarhoven_test)