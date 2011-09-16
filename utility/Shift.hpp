#pragma once
#include "../bit/Bit.hpp"
#include "../board/Traits.hpp"

namespace dctl {

namespace { enum { L, R }; }

// template function object for uniform left/right bitwise shift
template<bool>
struct Shift;

// specialization for bitwise shift-left
template<>
struct Shift<L>
{
        template<typename T> 
        T operator()(T square, int dir) const
        {
                return square << dir;
        }
};

// specialization for bitwise shift-right
template<>
struct Shift<R>
{
        template<typename T> 
        T operator()(T square, int dir) const
        {
                return square >> dir;
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<bool>
struct ShiftAssign;

// specialization for bitwise shift-left assignment
template<>
struct ShiftAssign<L>
{
        template<typename T> 
        void operator()(T& square, int dir) const
        {
                square <<= dir;
        }
};

// specialization for bitwise shift-right assignment
template<>
struct ShiftAssign<R>
{
        template<typename T> 
        void operator()(T& square, int dir) const
        {
                square >>= dir;
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, int Index>
struct Push
{
        template<typename T> 
        T operator()(T square) const
        {
                return Shift<board::is_positive<Index>::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, int Index>
struct Pull
{
        template<typename T> 
        T operator()(T square) const
        {
                return Shift<board::is_negative<Index>::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PushAssign
{
        template<typename T> 
        void operator()(T& square) const
        {
                ShiftAssign<board::is_positive<Index>::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PullAssign
{
        template<typename T> 
        void operator()(T& square) const
        {
                ShiftAssign<board::is_negative<Index>::value>()(square, Board::SHIFT[Index]);
        }
};

template<typename Board, int Index>
struct FloodFill
{
        template<typename T> 
        T operator()(T generator, T propagator) const
        {
                return bit::flood_fill<board::is_positive<Index>::value>(generator, propagator, Board::SHIFT[Index]);
        }
};

}       // namespace dctl
