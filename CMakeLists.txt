cmake_minimum_required(VERSION 3.13)
project(dctl CXX)

set(project_include_dir
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

set(xstd_include_dir ${PROJECT_SOURCE_DIR}/../xstd/include)
set(bit_set_include_dir ${PROJECT_SOURCE_DIR}/../bit_set/include)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.69.0 REQUIRED COMPONENTS
	filesystem
	program_options
	unit_test_framework
)

add_library(dctl INTERFACE)
target_include_directories(dctl INTERFACE
	${project_include_dir}
	${xstd_include_dir}
	${bit_set_include_dir}
)

target_compile_features(dctl INTERFACE cxx_std_20)

add_subdirectory(example)
include(CTest)
add_subdirectory(test)
