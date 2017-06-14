###############################################################
# use the Boost.Test unit test framework
###############################################################

list(APPEND cxx_compile_definitions_boost_test 
        BOOST_TEST_DYN_LINK 
        BOOST_TEST_MAIN
)

list(APPEND cxx_compile_options_warnings_boost_test 
        $<$<CXX_COMPILER_ID:Clang>:
                -Wno-disabled-macro-expansion 
                -Wno-global-constructors 
        >
)

###############################################################
# common compiler and linker flags
###############################################################

list(APPEND cxx_compile_options_warnings_general
        $<$<CXX_COMPILER_ID:Clang>:
                -Weverything 
                -Wno-c++98-compat 
                -Wno-c++98-compat-pedantic
                -Wno-error=unused-lambda-capture
                -Wno-padded
        >
        $<$<CXX_COMPILER_ID:GNU>:
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion                
                -Wshadow
                -Wsign-compare 
                -Wsign-promo
                -Wno-error=sign-conversion 
        >
 	-Werror 
        -pedantic-errors
)  

list(APPEND cxx_compile_options_optimization 
        -g
        -O3
        -flto 
        -march=native
        -mtune=native
)

list(APPEND cxx_linker_optimization_flags
        -flto 
        -fuse-ld=gold
)

###############################################################
# define test executables
###############################################################

set(test_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(test_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE targets RELATIVE ${test_source_dir} *.cpp)

foreach(t ${targets})
	get_filename_component(target_path ${t} PATH)
	get_filename_component(target_name_we ${t} NAME_WE)
	string(REPLACE "/" "." target_id ${target_path}/${target_name_we})
	string(REGEX REPLACE "^[.]" "" target_id ${target_id})

	add_executable(${target_id} src/${t})
        target_include_directories(${target_id} PRIVATE
                ${test_include_dir}
        )

        target_link_libraries(${target_id} PRIVATE
                dctl 
                ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                ${cxx_linker_optimization_flags}
        )

        target_compile_definitions(${target_id} PRIVATE 
                ${cxx_compile_definitions_boost_test}
        )

        target_compile_options(${target_id} PRIVATE 
                ${cxx_compile_options_warnings_general}
                ${cxx_compile_options_warnings_boost_test}
                ${cxx_compile_options_optimization}     
        )

        add_test(${target_id} ${target_id})    
endforeach()

