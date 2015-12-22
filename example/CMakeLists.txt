set( Boost_USE_STATIC_LIBS ON ) 
find_package(Boost REQUIRED program_options)

###############################################################
# executables
###############################################################

file(GLOB_RECURSE targets RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(t ${targets})
  get_filename_component(target_path ${t} PATH)
  get_filename_component(target_name_we ${t} NAME_WE)
  string(REPLACE "/" "." target_id ${target_path}/${target_name_we})
  string(REGEX REPLACE "^[.]" "" target_id ${target_id})

  add_executable(${target_id} ${t})
  target_link_libraries(${target_id} ${Boost_LIBRARIES})
endforeach()
