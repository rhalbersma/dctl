###############################################################
# Project settings
###############################################################

cmake_minimum_required(VERSION 2.8)
project(DCTL CXX)

###############################################################
# Compiler settings
###############################################################

# use C++17 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")# -stdlib=libc++")

# fail hard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors")

# highest general warning level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")

# NOT A DEFECT: no C++98 compatibility
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")

# NOT A DEFECT: padding is a diagnostic only (useful to turn on during future optimizations)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")

# KNOWN ISSUE: Boost.Test does some macro magic that requires static initialization
# http://hadesmem.googlecode.com/svn-history/r1861/trunk/tests/injector.cpp
# http://stackoverflow.com/a/15708829/819272 for a rationale for this warning by Howard Hinnant
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion")

# TODO: DXP message classes with virtual functions should not be header-only
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

# optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -mtune=native")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")# -fuse-ld=gold")

###############################################################
# DEPENDENCIES
###############################################################

set(project_include_dirs ${PROJECT_SOURCE_DIR}/include)
include_directories(${project_include_dirs})

#-----------------------------------------------
# modified and extended Standard Library headers
#-----------------------------------------------
set(XSTD_include_dirs ${PROJECT_SOURCE_DIR}/../xstd/include)
include_directories(${XSTD_include_dirs})

#--------------------------------
# Eric Niebler's range-v3 library
#--------------------------------
set(RANGE_V3_include_dirs ${PROJECT_SOURCE_DIR}/../range-v3/include)
include_directories(SYSTEM ${RANGE_V3_include_dirs})
add_definitions(-DRANGES_SUPPRESS_IOTA_WARNING)

#----------------
# Boost libraries
#----------------
find_package(Boost 1.60.0 REQUIRED COMPONENTS chrono system timer unit_test_framework)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
add_definitions(-D BOOST_ALL_DYN_LINK)
add_definitions(-D BOOST_TEST_MAIN)

###############################################################
# COMPONENTS
###############################################################

add_subdirectory(example)
include(CTest)          # calls enable_testing()
add_subdirectory(test)  # see CMakeLists.txt in test/ subdirectory
