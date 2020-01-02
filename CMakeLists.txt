#          Copyright Rein Halbersma 2010-2020.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.15)
project(
    dctl
    HOMEPAGE_URL https://github.com/rhalbersma/${PROJECT_NAME}
    LANGUAGES CXX
)

add_library(
    ${PROJECT_NAME} INTERFACE
)

find_package(
    Boost REQUIRED COMPONENTS
    filesystem
)
target_link_libraries(
    ${PROJECT_NAME} INTERFACE
    Boost::filesystem
)

set(project_include_dir
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    ${project_include_dir}
    ${PROJECT_SOURCE_DIR}/../xstd/include
    ${PROJECT_SOURCE_DIR}/../bit_set/include
)

target_compile_features(
    ${PROJECT_NAME} INTERFACE
    cxx_std_17
)

include(CTest)
add_subdirectory(test)
add_subdirectory(example)
