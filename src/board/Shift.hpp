#pragma once
#include "boost/mpl/int_fwd.hpp"        // int_
#include "../board/Angle.hpp"
#include "../board/Traits.hpp"
#include "../rules/Enum.hpp"

namespace dctl {
namespace angle {

template<typename Board, typename Index>
struct shift_size;

template<typename Board> struct shift_size<Board, D000 >: boost::mpl::int_< Board::InternalGrid::right      > {};
template<typename Board> struct shift_size<Board, D045 >: boost::mpl::int_< Board::InternalGrid::right_up   > {};
template<typename Board> struct shift_size<Board, D090 >: boost::mpl::int_< Board::InternalGrid::up         > {};
template<typename Board> struct shift_size<Board, D135 >: boost::mpl::int_< Board::InternalGrid::left_up    > {};
template<typename Board> struct shift_size<Board, D180 >: boost::mpl::int_< Board::InternalGrid::left       > {};
template<typename Board> struct shift_size<Board, D225 >: boost::mpl::int_< Board::InternalGrid::left_down  > {};
template<typename Board> struct shift_size<Board, D270 >: boost::mpl::int_< Board::InternalGrid::down       > {};
template<typename Board> struct shift_size<Board, D315 >: boost::mpl::int_< Board::InternalGrid::right_down > {};

}       // namespace angle

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
                        angle::is_positive< Index >::value,
                        angle::shift_size<Board, Index>::value
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
                        angle::is_positive< Index >::value,
                        angle::shift_size<Board, Index>::value
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
                        angle::is_negative< Index >::value,
                        angle::shift_size<Board, Index>::value
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
