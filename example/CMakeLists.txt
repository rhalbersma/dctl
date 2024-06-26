#          Copyright Rein Halbersma 2010-2023.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


set(cxx_compile_definitions
    $<$<CXX_COMPILER_ID:MSVC>:
        _SCL_SECURE_NO_WARNINGS
    >
)


set(cxx_compile_options_warnings
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /permissive-
        /WX
        /wd4244                             # triggered by 32/64 bit incompatibilities
        /wd4458                             # triggered by types hiding data members
        /wd4459                             # triggered by Boost.Accumulators
        /wd4805                             # triggered by bit-wise XOR on booleans
    >
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror
        -pedantic-errors
    >    
    $<$<CXX_COMPILER_ID:Clang>:
        -Weverything
        -Wno-unknown-warning-option         # forward compatibility for new Clang warnings
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-bitwise-instead-of-logical
        -Wno-covered-switch-default
        -Wno-ctad-maybe-unsupported         # triggered by <> elision for <functional> operators
        -Wno-unsafe-buffer-usage
        -Wno-disabled-macro-expansion       # triggered by Boost.Test
        -Wno-global-constructors            # triggered by Boost.Test
        -Wno-used-but-marked-unused         # triggered by Boost.Test
        -Wno-zero-as-null-pointer-constant  # triggered by Boost.Test
        -Wno-padded                         # only affects performance
    >
    $<$<CXX_COMPILER_ID:GNU>:
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wnrvo
        -Wshadow
        -Wsign-compare
        -Wsign-conversion
        -Wsign-promo
        -Wno-maybe-uninitialized            # triggered by Boost.Graph
    >
)

set(cxx_compile_options_optimization
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -march=native
    >
)

set(cxx_compile_options_mbig_obj
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<PLATFORM_ID:Windows>>:
	    -Wa,-mbig-obj
    >
)

set(current_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE targets RELATIVE ${current_source_dir} *.cpp)

foreach(t ${targets})
    get_filename_component(target_path ${t} PATH)
    get_filename_component(target_name_we ${t} NAME_WE)
    string(REPLACE "/" "." target_id ${target_path}/${target_name_we})
    string(REGEX REPLACE "^[.]" "" target_id ${target_id})

    add_executable(${target_id} src/${t})

    target_link_libraries(
        ${target_id} PRIVATE
        ${CMAKE_PROJECT_NAME}
    )

    target_compile_definitions(
        ${target_id} PRIVATE
        ${cxx_compile_definitions}
    )

    target_compile_options(
        ${target_id} PRIVATE
        ${cxx_compile_options_warnings}
        ${cxx_compile_options_optimization}
        ${cxx_compile_options_mbig_obj}
    )
endforeach()
