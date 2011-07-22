#pragma once
#include <cstddef>

namespace dctl {

// template function object for uniform left/right bitwise shift
template<bool>
struct Shift
{
        template<typename T> 
        T operator()(T, size_t) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<bool>
struct ShiftAssign
{
        template<typename T> 
        void operator()(T&, size_t) const;
};

// template function object for uniform left/right bitwise shift
template<typename, size_t>
struct Push
{
        template<typename T> 
        T operator()(T) const;
};

// template function object for uniform left/right bitwise shift
template<typename, size_t>
struct Pull
{
        template<typename T> 
        T operator()(T) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<typename, size_t>
struct PushAssign
{
        template<typename T> 
        void operator()(T&) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<typename, size_t>
struct PullAssign
{
        template<typename T> 
        void operator()(T&) const;
};

template<typename, size_t>
struct FloodFill
{
        template<typename T> 
        T operator()(T, T) const;
};

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Shift.hpp"
