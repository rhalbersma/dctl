###############################################################
# use the Boost.Test unit test framework
###############################################################

set(cxx_compile_definitions_boost 
        BOOST_TEST_DYN_LINK 
        BOOST_TEST_MAIN
)

set(cxx_compile_options_warnings_boost 
        $<$<CXX_COMPILER_ID:Clang>:
                -Wno-disabled-macro-expansion 
                -Wno-global-constructors 
        >
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
#        -fprofile-generate
#        -fprofile-use 
#        -stdlib=libc++
        -g
        -O3
        -flto
        -march=native
        -mtune=native
)

set(cxx_linker_optimization_flags
#	--coverage
#        -fprofile-generate
#        -fprofile-use 
#		-stdlib=libc++
#		-lc++abi
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

        target_include_directories(${target_id} SYSTEM PRIVATE 
        		${Boost_INCLUDE_DIR}
        )

        target_link_libraries(${target_id} PRIVATE
                dctl 
                ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                ${cxx_linker_optimization_flags}
        )

        target_compile_definitions(${target_id} PRIVATE 
                ${cxx_compile_definitions_boost}
        )

        target_compile_options(${target_id} PRIVATE 
                ${cxx_compile_options_warnings_general}
                ${cxx_compile_options_warnings_boost}
                ${cxx_compile_options_optimization}     
        )

        add_test(${target_id} ${target_id})    
endforeach()
