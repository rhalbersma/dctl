cmake_minimum_required(VERSION 3.11)
project(dctl CXX)

set(project_include_dir
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

set(xstd_include_dir ${PROJECT_SOURCE_DIR}/../xstd/include)
set(ext_include_dir ${PROJECT_SOURCE_DIR}/../ext)

find_package(Boost 1.66.0 REQUIRED COMPONENTS
	program_options
	unit_test_framework
)

add_library(dctl INTERFACE)
target_include_directories(dctl INTERFACE 
	${project_include_dir}
	${xstd_include_dir}
	${ext_include_dir}
)
target_link_libraries(dctl INTERFACE -lstdc++fs)
target_compile_features(dctl INTERFACE cxx_std_17)

add_subdirectory(example)
include(CTest)
add_subdirectory(test)
