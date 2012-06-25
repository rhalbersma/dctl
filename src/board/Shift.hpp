#pragma once
#include "boost/mpl/int_fwd.hpp"        // int_
#include "../board/Angle.hpp"
#include "../board/Traits.hpp"
#include "../rules/Enum.hpp"

namespace dctl {

// primary template
template<typename Board, typename Index, typename Range>
struct Sink;

template<typename Board, typename Index>
struct Sink<Board, Index, rules::range::distance_1>
{
        template<typename T>
        T operator()(T from, T dest) const
        {
                return Board::next<Index>(from) & dest;
        }
};

template<typename Board, typename Index>
struct Sink<Board, Index, rules::range::distance_N>
{
        template<typename T>
        T operator()(T from, T dest) const
        {
                return from ^ FloodFill<Board, Index>()(from, dest);
        }
};

// primary template
template<typename Board, typename Index, typename Range>
struct Sandwich;

template<typename Board, typename Index>
struct Sandwich<Board, Index, rules::range::distance_1>
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

template<typename Board, typename Index>
struct Sandwich<Board, Index, rules::range::distance_N>
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

template<typename Board, typename Index>
struct FloodFill
{
        template<typename T>
        T operator()(T generator, T propagator) const
        {
                return flood_fill<
                        angle::is_positive<Index>::value,
                        Board::shift<Index>::value
                >(generator, propagator);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, typename Index>
struct Push
{
        template<typename Iterator>
        Iterator operator()(Iterator square) const
        {
                return Shift<
                        angle::is_positive<Index>::value,
                        Board::shift<Index>::value
                >()(square);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, typename Index>
struct Pull
{
        template<typename Iterator>
        Iterator operator()(Iterator square) const
        {
                return Shift<
                        angle::is_negative<Index>::value,
                        Board::shift<Index>::value
                >()(square);
        }
};

// direction-wise flood-fill generator over propagator
template<bool Sign, int N, typename T>
T flood_fill(T generator, T propagator)
{
        return fill_loop<Sign, N>(generator, propagator);
}

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, int N, typename T>
T fill_loop(T generator, T propagator)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign, N>()(generator) & propagator;
        }
        return flood;
}

// left and right direction
enum { L, R };

// primary template
template<bool, int>
struct Shift;

// partial specialization for bitwise shift-left
template<int N>
struct Shift<L, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return square << N;
        }
};

// partial specialization for bitwise shift-right
template<int N>
struct Shift<R, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return square >> N;
        }
};

// primary template
template<bool, int>
struct ShiftAssign;

// partial specialization for bitwise shift-left assignment
template<int N>
struct ShiftAssign<L, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square <<= N;
        }
};

// partial specialization for bitwise shift-right assignment
template<int N>
struct ShiftAssign<R, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square >>= N;
        }
};

}       // namespace dctl
