###############################################################
# Project settings
###############################################################

cmake_minimum_required(VERSION 2.8)
project(DCTL CXX)

###############################################################
# Compiler settings
###############################################################

# use C++14 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=512")

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -march=native -mtune=native")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi -flto -fuse-ld=gold")

###############################################################
# DEPENDENCIES
###############################################################

# Boost libraries
#----------------
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-D BOOST_TEST_MAIN)
add_definitions(-D BOOST_ALL_DYN_LINK)

# Eric Niebler's range-v3 library
#--------------------------------
set(RANGE_V3_include_dirs ${PROJECT_SOURCE_DIR}/../range-v3/include)
include_directories(SYSTEM ${RANGE_V3_include_dirs})
add_definitions(-DRANGES_SUPPRESS_IOTA_WARNING)

# modified and extended Standard Library headers
#-----------------------------------------------
set(XSTD_include_dirs ${PROJECT_SOURCE_DIR}/../xstd/include)
include_directories(${XSTD_include_dirs})

###############################################################
# MyAwesomeLibrary headers
###############################################################

set(MyAwesomeLibrary_include_dirs ${PROJECT_SOURCE_DIR}/include)
include_directories(${MyAwesomeLibrary_include_dirs})
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
# TESTING
###############################################################

include(CTest)          # calls enable_testing()
add_subdirectory(test)  # see CMakeLists.txt in test/ subdirectory
