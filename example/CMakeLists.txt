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
        /wd4244                         # triggered by 32/64 bit incompatibilities
        /wd4458                         # triggered by types hiding data members
        /wd4459                         # triggered by Boost.Accumulators
        /wd4805                         # triggered by bit-wise XOR on booleans
    >
    $<$<CXX_COMPILER_ID:Clang>:
        -Weverything
        -Wno-unknown-warning-option     # forward compatibility for new Clang warnings
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-ctad-maybe-unsupported     # triggered by <> elision for <functional> operators
        -Wno-global-constructors        # triggered by immediately invoked lambda tables
        -Wno-padded                     # only affects performance
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
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror
        -pedantic-errors
    >
)

set(cxx_compile_options_mbig_obj
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<PLATFORM_ID:Windows>>:
	    -Wa,-mbig-obj
    >
)

set(current_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
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

    target_include_directories(
        ${target_id} PRIVATE
        ${current_include_dir}
    )

    target_compile_definitions(
        ${target_id} PRIVATE
        ${cxx_compile_definitions}
    )

    target_compile_options(
        ${target_id} PRIVATE
        ${cxx_compile_options_warnings}
        ${cxx_compile_options_mbig_obj}
    )
endforeach()
