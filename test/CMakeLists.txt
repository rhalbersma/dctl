###############################################################
# Test project settings
###############################################################

# require a recent CMake version
cmake_minimum_required(VERSION 2.8)

# this is a C++ only project
project(TestProject CXX)

###############################################################
# Compiler settings
###############################################################

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# general warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -pedantic-errors")

# integral conversion and promotion warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion -Wsign-promo")

# optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wdisabled-optimization -march=native")

###############################################################
# Boost libraries
###############################################################

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# dynamically link against Boost
add_definitions(-D BOOST_ALL_DYN_LINK)

# define a main() entry point for each test
add_definitions(-D BOOST_TEST_MAIN)

###############################################################
# Parent project headers
###############################################################

# locate the project under test
set(ParentProject_include_dirs ${PROJECT_SOURCE_DIR}/../include)

# find the parent project headers
file(GLOB_RECURSE ParentProject_headers ${ParentProject_include_dirs}/*.hpp)

###############################################################
# Standalone parent project headers
###############################################################

# to enable this part: cmake -D CheckStandalone=true
if(CheckStandalone)
  # compile with project wide compiler settings
  set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

  # compile against the parent project headers
  set(CMAKE_REQUIRED_INCLUDES ${ParentProject_include_dirs})

  # enable standalone testing of the header files
  include(CheckIncludeFileCXX)

  foreach(h ${ParentProject_headers})
    # header name without extension
    get_filename_component(header_name ${h} NAME_WE)

    # check if header is standalone
    check_include_file_cxx(${h} ${header_name}_is_standalone)

    # fail early, fail hard
    if(NOT ${header_name}_is_standalone)
      message(FATAL_ERROR "Found a non-standalone header (see CMakeFiles/CMakeError.log for details).")
    endif()
  endforeach()
endif()

###############################################################
# Test suite
###############################################################

# get the test sources
file(GLOB_RECURSE test_sources RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)

# except any CMake generated sources under build/
string(REGEX REPLACE "build/[^;]+;?" "" test_sources "${test_sources}")

# get the test headers
file(GLOB_RECURSE test_headers RELATIVE ${PROJECT_SOURCE_DIR} *.hpp)

# except any CMake generated headers under build/
string(REGEX REPLACE "build/[^;]+;?" "" test_headers "${test_headers}")

# compile against the test headers, the parent project, and the Boost libraries
include_directories(${PROJECT_SOURCE_DIR} ${ParentProject_include_dirs} ${Boost_INCLUDE_DIRS})

# calls enable_testing()
include(CTest)

foreach(t ${test_sources} )
  # get the relative path in the source tree
  get_filename_component(test_path ${t} PATH)

  # get the source name without extension
  get_filename_component(test_name ${t} NAME_WE)

  # concatenate the relative path and name in an underscore separated identifier
  string(REPLACE "/" "_" test_concat "${test_path}/${test_name}")

  # strip the leading "test_" part from the test ID
  string(REGEX REPLACE "^test_" "" test_id ${test_concat})

  # depend on the current source file, all the test headers, and the parent project headers
  add_executable(${test_id} ${t} ${test_headers} ${ParentProject_headers})

  # link against the Boost libraries
  target_link_libraries(${test_id} ${Boost_LIBRARIES})

  # match the relative path in the build tree with the corresponding one in the source tree 
  set_target_properties(${test_id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${test_path})

  # add a test with executable in the relative path of the build tree
  add_test(${test_id} ${test_path}/${test_id})
endforeach()

