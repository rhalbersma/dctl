###############################################################
# Project settings
###############################################################

# require a recent CMake version
cmake_minimum_required(VERSION 2.8)

# this is a C++ only project
project(MyAwesomeLibraryTestSuite CXX)

###############################################################
# Compiler settings
###############################################################

# use C++1y features
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++ -ftemplate-depth=1024")

# fail hard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors")

# highest general warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")

# NOT A DEFECT: no C++98 compatibility
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")

# NOT A DEFECT: padding is a diagnostic only (useful to turn on during future optimizations)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")

# KNOWN ISSUE: Boost.Test does some macro magic that requires static initialization
# http://hadesmem.googlecode.com/svn-history/r1861/trunk/tests/injector.cpp
# http://stackoverflow.com/a/15708829/819272 for a rationale for this warning by Howard Hinnant
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")

# TODO: DXP message classes with virtual functions should not be header-only
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")

# optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -march=native")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi -flto -fuse-ld=gold")

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
# modified and extended Standard Library headers
###############################################################

set(XSTD_include_dirs ${PROJECT_SOURCE_DIR}/../../xstd/include)

###############################################################
# MyAwesomeLibrary headers
###############################################################

# locate the library that is being tested
set(MyAwesomeLibrary_include_dirs ${PROJECT_SOURCE_DIR}/../include)

# find the parent project headers
file(GLOB_RECURSE MyAwesomeLibrary_headers ${MyAwesomeLibrary_include_dirs}/*.hpp)

###############################################################
# Check whether MyAwesomeLibrary headers are standalone
###############################################################

# to enable this part: cmake -D CheckStandalone=true

if(CheckStandalone)
  # compile with project wide compiler settings
  set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

  # compile against the parent project headers
  set(CMAKE_REQUIRED_INCLUDES ${MyAwesomeLibrary_include_dirs} ${XSTD_include_dirs})

  # enable standalone testing of the header files
  include(CheckIncludeFileCXX)

  foreach(h ${MyAwesomeLibrary_headers})
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

# calls enable_testing()
include(CTest)

# get the test headers
file(GLOB_RECURSE MyAwesomeLibraryTestSuite_headers RELATIVE ${PROJECT_SOURCE_DIR} src/*.hpp)

# compile against the test headers, the parent project, the XSTD headers, and the Boost libraries
include_directories(${PROJECT_SOURCE_DIR}/src ${MyAwesomeLibrary_include_dirs} ${XSTD_include_dirs} ${Boost_INCLUDE_DIRS})

# get the test sources
file(GLOB_RECURSE MyAwesomeLibraryTestSuite_sources RELATIVE ${PROJECT_SOURCE_DIR} src/*.cpp)

foreach(t ${MyAwesomeLibraryTestSuite_sources})
  # get the relative path in the source tree
  get_filename_component(test_path ${t} PATH)
 
  # strip the leading "src/" part from the test path
  string(REGEX REPLACE "^src[/]?" "" test_rel_path ${test_path})
  
  # get the source name without extension
  get_filename_component(test_name ${t} NAME_WE)
  
  # concatenate the relative path and name in an underscore separated identifier
  if ("${test_rel_path}" STREQUAL "")
    string(REPLACE "/" "_" test_id "${test_name}")
  else() 
    string(REPLACE "/" "_" test_id "${test_rel_path}/${test_name}")
  endif()

  # depend on the current source file, all the test headers, and the parent project headers
  add_executable(${test_id} ${t} ${MyAwesomeLibraryTestSuite_headers} ${MyAwesomeLibrary_headers})

  # link against the Boost libraries
  target_link_libraries(${test_id} ${Boost_LIBRARIES})

  # match the relative path in the build tree with the corresponding one in the source tree
  set_target_properties(${test_id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${test_path})
  
  # add a test with executable in the relative path of the build tree
  add_test(${test_id} ${test_path}/${test_id})  
endforeach()
