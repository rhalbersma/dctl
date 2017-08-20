###############################################################
# use the Boost.Test unit test framework
###############################################################

set(cxx_compile_definitions_boost 
        BOOST_ALL_DYN_LINK 
)

###############################################################
# common compiler and linker flags
###############################################################

set(cxx_compile_options_warnings_general
        $<$<CXX_COMPILER_ID:Clang>:
                -Weverything 
                -Wno-c++98-compat 
                -Wno-c++98-compat-pedantic
                -Wno-padded
        >
        $<$<CXX_COMPILER_ID:GNU>:
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion                
                -Wshadow
                -Wsign-compare 
                -Wsign-conversion
                -Wsign-promo
        >
 		-Werror 
        -pedantic-errors
)  

set(cxx_compile_options_optimization 
#	--coverage
#		-fwhole-program
#        -fprofile-generate
#        -fprofile-use 
        -g
        -O3
        -flto
        -march=native
        -mtune=native
)

set(cxx_linker_optimization_flags
#	--coverage
#		-fwhole-program
#        -fprofile-generate
#        -fprofile-use 
        -flto 
        -fuse-ld=gold
)

###############################################################
# define test executables
###############################################################

set(test_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE targets RELATIVE ${test_source_dir} *.cpp)

foreach(t ${targets})
		get_filename_component(target_path ${t} PATH)
		get_filename_component(target_name_we ${t} NAME_WE)
		string(REPLACE "/" "." target_id ${target_path}/${target_name_we})
		string(REGEX REPLACE "^[.]" "" target_id ${target_id})

		add_executable(${target_id} src/${t})
	
        target_include_directories(${target_id} PRIVATE
        )

        target_link_libraries(${target_id} PRIVATE
                dctl 
                ${Boost_PROGRAM_OPTIONS_LIBRARY}
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
endforeach()
