cmake_minimum_required(VERSION 2.8)
project(dctl_test)

###############################################################
# Compiler settings
###############################################################

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# set warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -pedantic-errors -Wall -Wextra")

###############################################################
# Boost libraries
###############################################################

# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

###############################################################
# DCTL library
###############################################################

# locate the DCTL library
set(dctl_include_dirs ${PROJECT_SOURCE_DIR}/../include)

# the DCTL headers
file(GLOB_RECURSE dctl_headers FOLLOW_SYMLINKS "${dctl_include_dirs}/*.hpp")

###############################################################
# Test suite
###############################################################

# the test suite
file(GLOB_RECURSE test_cases FOLLOW_SYMLINKS "test/*.[h,c]pp")

# the test runner depends on the test suite and the DCTL headers
add_executable(test_suite ${test_cases} ${dctl_headers})

# compile against test headers, DCTL headers, and Boost headers
include_directories(${PROJECT_SOURCE_DIR} ${dctl_include_dirs} ${Boost_INCLUDE_DIRS})

# link against Boost.Test
target_link_libraries(test_suite ${Boost_LIBRARIES})

###############################################################
# Test runner
###############################################################

# calls "enable_testing()", allows "make test" or "ctest"
include(CTest)

# run the test suite
add_test(test_runner test_suite)
