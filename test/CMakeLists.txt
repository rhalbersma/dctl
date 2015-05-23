###############################################################
# test headers
###############################################################

set(test_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${test_include_dirs})

###############################################################
# test sources
###############################################################

set(test_source_tree ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE test_sources RELATIVE ${test_source_tree} *.cpp)

###############################################################
# test suite
###############################################################

foreach(t ${test_sources})
  get_filename_component(test_path ${t} PATH)
  get_filename_component(test_name_we ${t} NAME_WE)
  string(REPLACE "/" "." test_id ${test_path}/${test_name_we})

  add_executable(${test_id} src/${t})
  target_link_libraries(${test_id} ${Boost_LIBRARIES})
  set_target_properties(${test_id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${test_path})
  add_test(test.${test_id} ${test_path}/${test_id})    
endforeach()
