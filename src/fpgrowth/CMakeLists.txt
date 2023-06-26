add_executable(fim_maximal
  main.cpp
  fpmax.cpp
  data.cpp
  fp_tree.cpp
  buffer.cpp
  fsout.cpp
  fp_node.cpp
)

target_include_directories(fim_maximal
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_compile_definitions(fim_maximal
  PRIVATE MFI)
