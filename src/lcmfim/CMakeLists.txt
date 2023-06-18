add_library(lcm-fim
  lcm_max.c
)

target_include_directories(lcm-fim
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
