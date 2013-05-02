cmake_minimum_required(VERSION 2.8)

project(dctl_test)

# use C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)

# locate the Boost libraries
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(Boost_FOUND)
  # add all test cases to the testsuite
  file(GLOB_RECURSE testsuite FOLLOW_SYMLINKS "*.hpp" "*.cpp")

  # build the testrunner...
  add_executable(testrunner ${testsuite})

  # ...by compiling against the project + Boost headers...
  include_directories(.. ${Boost_INCLUDE_DIRS})

  # ...and linking against Boost.Test
  target_link_libraries(testrunner ${Boost_LIBRARIES})

  # run the testrunner
  get_target_property(testrunner_EXE testrunner LOCATION)
  add_custom_target(
    test ${testrunner_EXE} --catch_system_error=yes --report_level=no --log_level=nothing
    DEPENDS testrunner
  )
endif()
