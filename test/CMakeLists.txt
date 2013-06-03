cmake_minimum_required(VERSION 2.8)
project(dctl_test)

###############################################################
# Compiler settings
###############################################################

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# general warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -pedantic-errors")

# integral conversion and promotion warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion -Wsign-promo")

# optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wdisabled-optimization")

###############################################################
# Boost libraries
###############################################################

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# dynamically link against Boost
add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_TEST_MAIN)

###############################################################
# DCTL library
###############################################################

# locate the DCTL library
set(dctl_include_dirs ${PROJECT_SOURCE_DIR}/../include)

# the DCTL headers
file(GLOB_RECURSE dctl_headers ${dctl_include_dirs}/*.hpp)

###############################################################
# Standalone headers
###############################################################

# cmake -DCheckStandalone=true runs this test
if(CheckStandalone)
  set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_REQUIRED_INCLUDES ${dctl_include_dirs})
  include(CheckIncludeFileCXX)

  foreach(h ${dctl_headers})
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

include(CTest)
include_directories(${PROJECT_SOURCE_DIR} ${dctl_include_dirs} ${Boost_INCLUDE_DIRS})


file(GLOB_RECURSE test_suites RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)

string(REGEX REPLACE "build/[^;]+;?" "" test_suites "${test_suites}")

foreach(t ${test_suites} )
    # Get the path to the test-case, relative to the ${CMAKE_CURRENT_SOURCE_DIR} 
    # thanks to the RELATIVE flag in file( GLOB_RECURSE ... )
    get_filename_component(test_path ${t} PATH)

    # Get the name without extension
    get_filename_component(test_name ${t} NAME_WE)

    # construct a 'unique' test-name
    string(REPLACE "/" "_" full_testcase "${test_path}/${test_name}")

    # Add an executable using the 'unique' test-name
    message(STATUS "  added " ${full_testcase})

    # depend on test sources and the DCTL headers
    add_executable(${full_testcase} ${t} ${dctl_headers})

    # and modify its output paths. 
    set_target_properties(${full_testcase} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${test_path} )
    target_link_libraries(${full_testcase} ${Boost_LIBRARIES})

    add_test(${full_testcase} ${test_path}/${full_testcase})
endforeach()

