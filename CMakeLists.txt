cmake_minimum_required(VERSION 3.12)

project(grasp-mining 
	VERSION 0.0
	LANGUAGES CXX
)

add_executable(grasp
	src/Main.cpp
	src/data/bierwirth_sequence.cpp
	src/data/problem.cpp
	src/local-search/laarhoven.cpp
	src/solver/solver.cpp
	src/util/MT.cpp
	src/util/Out_JS.cpp
)

target_include_directories(grasp
	PUBLIC src/
)