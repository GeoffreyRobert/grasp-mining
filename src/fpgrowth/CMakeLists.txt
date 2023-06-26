add_library(fp-max
  fpmax.cpp
  data.cpp
  fp_tree.cpp
  buffer.cpp
  fsout.cpp
  fp_node.cpp
)

target_include_directories(fp-max
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_compile_definitions(fp-max
  PRIVATE MFI)

add_executable(fim_maximal
  main.cpp
)

target_link_libraries(fim_maximal
  PRIVATE fp-max
)
