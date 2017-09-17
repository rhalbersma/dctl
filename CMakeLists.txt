###############################################################
# project settings
###############################################################

cmake_minimum_required(VERSION 3.9)
project(dctl CXX)

set(project_include_dir
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

###############################################################
# dependencies
###############################################################

#-----------------------------------------------
# modified and extended Standard Library headers
#-----------------------------------------------
set(xstd_include_dir ${PROJECT_SOURCE_DIR}/../xstd/include)

#-----------------------------------------------
# Howard Hinnant's hash_append
#-----------------------------------------------
set(ext_include_dir ${PROJECT_SOURCE_DIR}/../ext)

#----------------
# Boost libraries
#----------------
find_package(Boost 1.65.1 REQUIRED COMPONENTS 
	unit_test_framework
	program_options
)

###############################################################

add_library(dctl INTERFACE)
target_include_directories(dctl INTERFACE 
	${project_include_dir}
	${xstd_include_dir}
	${ext_include_dir}
)
target_link_libraries(dctl INTERFACE -lstdc++fs)
target_compile_features(dctl INTERFACE cxx_std_17)

###############################################################

#TODO: install()

###############################################################
# enable unit testing
###############################################################

add_subdirectory(example)
include(CTest)
add_subdirectory(test)
