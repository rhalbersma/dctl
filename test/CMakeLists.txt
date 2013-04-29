# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(Boost_FOUND)
  # add project headers + Boost headers
  include_directories(${DCTL_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

  # add all test cases
  file(GLOB_RECURSE tests FOLLOW_SYMLINKS "*.hpp" "*.cpp")
  add_executable(main ${tests})

  # link against Boost.Test
  target_link_libraries(main ${Boost_LIBRARIES})
endif()
