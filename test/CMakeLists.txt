cmake_minimum_required(VERSION 2.8)

project(dctl_test)

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# set warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -pedantic-errors -Wall -Wextra")

# template headers location
set(dctl_include_dirs ..)

# compile against the template headers
include_directories(${dctl_include_dirs})

# the template header files
file(GLOB_RECURSE dctl_template_headers RELATIVE ${dctl_include_dirs} FOLLOW_SYMLINKS "*.hpp")

# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# compile against Boost
include_directories(${Boost_INCLUDE_DIRS})

# the test suite
file(GLOB_RECURSE suite FOLLOW_SYMLINKS "*.hpp" "*.cpp")

# the test runner depends on the test suite and the project template headers
add_executable(runner ${suite} ${dctl_template_headers})

# link against Boost.Test
target_link_libraries(runner ${Boost_LIBRARIES})

# run the testrunner
get_target_property(runner_EXE runner LOCATION)
add_custom_target(
  test ${runner_EXE} --catch_system_error=yes --report_level=no --log_level=nothing
  DEPENDS runner
)
