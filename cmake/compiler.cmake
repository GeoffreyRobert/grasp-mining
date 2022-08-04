# Compiler specific settings

if(compiler_included)
  return()
endif()
set(compiler_included ON)

# Link a target to this library to specify its compiler settings
add_library(compiler_settings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(compiler_settings INTERFACE
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(compiler_settings INTERFACE
      -Wmisleading-indentation
      -Wduplicated-cond
      -Wduplicated-branches
      -Wlogical-op
      -Wuseless-cast)
  endif()
endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0")
