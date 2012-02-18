#pragma once
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Traits.hpp"
#include "../rules/Rules.hpp"
#include "../successor/Selection.hpp"

namespace dctl {

// TODO <dir> argument of Shift function can be made a template parameter in C++11 (using constexpr)

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
                return Shift<board::is_positive< angle<Index> >::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, int Index>
struct Pull
{
        template<typename T> 
        T operator()(T square) const
        {
                return Shift<board::is_negative< angle<Index> >::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PushAssign
{
        template<typename T> 
        void operator()(T& square) const
        {
                ShiftAssign<board::is_positive< angle<Index> >::value>()(square, Board::SHIFT[Index]);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PullAssign
{
        template<typename T> 
        void operator()(T& square) const
        {
                ShiftAssign<board::is_negative< angle<Index> >::value>()(square, Board::SHIFT[Index]);
        }
};

template<typename Board, int Index>
struct FloodFill
{
        template<typename T> 
        T operator()(T generator, T propagator) const
        {
                return bit::flood_fill<board::is_positive< angle<Index> >::value>(generator, propagator, Board::SHIFT[Index]);
        }
};

// primary template
template<typename Board, int Index, int Range>
struct Sink;

template<typename Board, int Index>
struct Sink<Board, Index, rules::scan_1>
{
        template<typename T> 
        T operator()(T from, T dest) const
        {
                return Push<Board, Index>()(from) & dest;
        }
};

template<typename Board, int Index>
struct Sink<Board, Index, rules::scan_N>
{
        template<typename T> 
        T operator()(T from, T dest) const
        {
                return from ^ FloodFill<Board, Index>()(from, dest);
        }
};

// primary template
template<typename Board, int Index, int Range>
struct Sandwich;

template<typename Board, int Index>
struct Sandwich<Board, Index, rules::scan_1>
{
        template<typename T> 
        T operator()(T from, T past, T dest) const
        {
                return (
                        Push<Board, Index>()(from) & 
                        past & 
                        Pull<Board, Index>()(dest)
                );
        }
};

template<typename Board, int Index>
struct Sandwich<Board, Index, rules::scan_N>
{
        template<typename T> 
        T operator()(T from, T past, T dest) const
        {
                return (
                        Push<Board, Index>()(FloodFill<Board, Index>()(from, dest)) & 
                        past & 
                        Pull<Board, Index>()(dest)
                );
        }
};

}       // namespace dctl
