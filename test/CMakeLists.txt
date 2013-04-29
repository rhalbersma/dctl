# statically link against Boost.Test
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(Boost_FOUND)
  # add project headers + Boost headers
  include_directories(${DCTL_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

  # add all test cases
  file(GLOB_RECURSE testsuite FOLLOW_SYMLINKS "*.hpp" "*.cpp")
  add_executable(testrunner ${testsuite})

  # link against Boost.Test
  target_link_libraries(testrunner ${Boost_LIBRARIES})

  get_target_property(testrunner_EXE testrunner LOCATION)
  add_custom_target(
    test ${testrunner_EXE} --result_code=yes --report_level=detailed
    DEPENDS testrunner
  )
endif()
