cmake_minimum_required(VERSION 2.8)

project(DCTL)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# out-of-source build of dctl/test into build/test
add_subdirectory(test test)

