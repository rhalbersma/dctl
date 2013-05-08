cmake_minimum_required(VERSION 2.8)
project(dctl_test)
include(CheckIncludeFile)
include(CheckIncludeFiles)

###############################################################
# Compiler settings
###############################################################

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# set warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -pedantic-errors -Wall -Wextra")

# template headers location
set(dctl_include_dirs ${PROJECT_SOURCE_DIR}/../include)

###############################################################
# Boost related stuff
################################################################

# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# compile against test headers, template library headers, and Boost headers
include_directories(${PROJECT_SOURCE_DIR} ${dctl_include_dirs} ${Boost_INCLUDE_DIRS})

# the template headers
#file(GLOB_RECURSE dctl_template_headers FOLLOW_SYMLINKS "../include/*.hpp")

message("printing headers")

# the test suite
file(GLOB_RECURSE suite FOLLOW_SYMLINKS "test/*.hpp" "test/*.cpp")

# the test headers
file(GLOB_RECURSE test_headers FOLLOW_SYMLINKS "test/*.hpp")

message("printing test headers")
message("${test_headers}")
foreach(f ${test_headers})
  message("test=${f}")
endforeach()

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
message("Settings=${CMAKE_REQUIRED_FLAGS}")
set(CMAKE_REQUIRED_INCLUDES ${PROJECT_SOURCE_DIR} ${dctl_include_dirs} ${Boost_INCLUDE_DIRS} "usr/include/")
message("Settings=${CMAKE_REQUIRED_INCLUDES}")
check_include_files("${test_headers}" HAS_TEST_STANDALONE)
#foreach(f ${test_headers})
  check_include_file("/home/rein/projects/dctl/test/xxx.hpp" FIX_XXX CFLAGS )
#endforeach()

# the test runner depends on the test suite and the project template headers
add_executable(runner ${suite} ${dctl_template_headers})

# link against Boost.Test
target_link_libraries(runner ${Boost_LIBRARIES})
