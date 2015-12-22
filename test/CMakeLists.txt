###############################################################
# Boost.Test
###############################################################

find_package(Boost REQUIRED unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-D BOOST_TEST_MAIN)
add_definitions(-D BOOST_TEST_DYN_LINK)

###############################################################
# include and source directories
###############################################################

set(test_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(test_source_dirs ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${test_include_dirs})

###############################################################
# executables and tests
###############################################################

file(GLOB_RECURSE targets RELATIVE ${test_source_dirs} *.cpp)

foreach(t ${targets})
  get_filename_component(target_path ${t} PATH)
  get_filename_component(target_name_we ${t} NAME_WE)
  string(REPLACE "/" "." target_id ${target_path}/${target_name_we})
  string(REGEX REPLACE "^[.]" "" target_id ${target_id})

  add_executable(${target_id} src/${t})
  target_link_libraries(${target_id} ${Boost_LIBRARIES})
  add_test(${target_id} ${target_id})    
endforeach()
