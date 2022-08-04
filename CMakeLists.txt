cmake_minimum_required(VERSION 3.12)

project(grasp-mining
  VERSION 0.0
  LANGUAGES CXX
)

#############
## Options ##
#############

option(ENABLE_INSTALL
  "Should ${PROJECT_NAME} be added to the install list? Useful in included using add_subdirectory." ON)

set(${PROJECT_NAME}_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")

##################
## Dependencies ##
##################

find_package(Boost)
find_package(GTest REQUIRED)
enable_testing()

#############
## Library ##
#############

add_library(grasp-lib
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

target_compile_features(grasp-lib
  PRIVATE cxx_std_11
)

target_include_directories(grasp-lib
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
  PUBLIC ${Boost_INCLUDE_DIRS}
)

################
## Exexutable ##
################

add_executable(grasp
  src/main.cpp
)

target_link_libraries(grasp
  PRIVATE grasp-lib
)

################
## Exexutable ##
################

# TODO: Link the tests to the library
add_executable(full_test
  tests/main_test.cpp
  tests/laarhoven_test.cpp
  tests/problem_test.cpp
)

# Copy test files to output directory
target_compile_definitions(full_test
  PRIVATE BINARY_DIR=${CMAKE_BINARY_DIR})
add_custom_command(
  TARGET full_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/instances
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/instances/laar_test.txt ${CMAKE_BINARY_DIR}/instances/laar_test.txt)

target_link_libraries(full_test
  PRIVATE grasp-lib GTest::gtest GTest::gtest_main)

add_test(NAME full_test COMMAND laarhoven_test)

###############
## Packaging ##
###############

if(ENABLE_INSTALL)
  install(
    TARGETS grasp
    EXPORT ${PROJECT_NAME}_Targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(
    EXPORT ${PROJECT_NAME}_Targets
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${${PROJECT_NAME}_INSTALL_CMAKEDIR})
endif()
