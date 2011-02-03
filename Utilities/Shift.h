#pragma once
#include "IntegerTypes.h"

// typedefs
enum { L, R };

// template function object for uniform left/right bitwise shift
template<bool>
struct Shift
{
        template<typename T> T operator()(T, size_t) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<bool>
struct ShiftAssign
{
        template<typename T> void operator()(T&, size_t) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Shift.hpp"
