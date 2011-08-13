#pragma once

namespace dctl {

// template function object for uniform left/right bitwise shift
template<bool>
struct Shift
{
        template<typename T> 
        T operator()(T, int) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<bool>
struct ShiftAssign
{
        template<typename T> 
        void operator()(T&, int) const;
};

// template function object for uniform left/right bitwise shift
template<typename, int>
struct Push
{
        template<typename T> 
        T operator()(T) const;
};

// template function object for uniform left/right bitwise shift
template<typename, int>
struct Pull
{
        template<typename T> 
        T operator()(T) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<typename, int>
struct PushAssign
{
        template<typename T> 
        void operator()(T&) const;
};

// template function object for uniform left/right bitwise shift-assignment
template<typename, int>
struct PullAssign
{
        template<typename T> 
        void operator()(T&) const;
};

template<typename, int>
struct FloodFill
{
        template<typename T> 
        T operator()(T, T) const;
};

}       // namespace dctl

// include template definitions inside header
#include "Shift.hpp"
