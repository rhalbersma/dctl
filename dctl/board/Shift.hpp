#pragma once
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/traits.hpp>
#include <dctl/rules/Enum.hpp>

namespace dctl {

// TODO: use C++11 template aliases
template<typename Board, typename Direction>
struct shift_size
:
        Board::template shift_size<Direction>
{};

// left and right direction

typedef boost::mpl::false_ L;
typedef boost::mpl::true_ R;

// primary template
template<typename, typename>
struct Shift;

// partial specialization for bitwise shift-left
template<typename N>
struct Shift<L, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return (square << N::value);
        }
};

// partial specialization for bitwise shift-right
template<typename N>
struct Shift<R, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return (square >> N::value);
        }
};

// primary template
template<typename, typename>
struct ShiftAssign;

// partial specialization for bitwise shift-left assignment
template<typename N>
struct ShiftAssign<L, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square <<= N::value;
        }
};

// partial specialization for bitwise shift-right assignment
template<typename N>
struct ShiftAssign<R, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square >>= N::value;
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, typename Direction>
struct Next
{
        template<typename Iterator>
        Iterator operator()(Iterator square) const
        {
                return (Shift< typename
                        angle::is_positive<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >()(square));
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, typename Direction>
struct Prev
{
        template<typename Iterator>
        Iterator operator()(Iterator square) const
        {
                return (Shift< typename
                        angle::is_negative<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >()(square));
        }
};

template<typename Board, typename Direction>
struct Advance
{
        template<typename Iterator>
        void operator()(Iterator& square) const
        {
                ShiftAssign< typename
                        angle::is_positive<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >()(square);
        }
};

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<typename Sign, typename N, typename T>
T fill_loop(T generator, T propagator)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign, N>()(generator) & propagator;
        }
        return (flood);
}

// direction-wise flood-fill generator over propagator
template<typename Sign, typename N, typename T>
T flood_fill(T generator, T propagator)
{
        return (fill_loop<Sign, N>(generator, propagator));
}

template<typename Board, typename Direction>
struct FloodFill
{
        template<typename T>
        T operator()(T generator, T propagator) const
        {
                return (flood_fill< typename
                        angle::is_positive<Direction>::type, typename 
                        shift_size<Board, Direction>::type
                >(generator, propagator));
        }
};

// primary template
template<typename Board, typename Direction, typename Range>
struct Sink;

template<typename Board, typename Direction>
struct Sink<Board, Direction, rules::range::distance_1>
{
        template<typename T>
        T operator()(T from, T dest) const
        {
                return (Next<Board, Direction>()(from) & dest);
        }
};

template<typename Board, typename Direction>
struct Sink<Board, Direction, rules::range::distance_N>
{
        template<typename T>
        T operator()(T from, T dest) const
        {
                return (from ^ FloodFill<Board, Direction>()(from, dest));
        }
};

// primary template
template<typename Board, typename Direction, typename Range>
struct Sandwich;

template<typename Board, typename Direction>
struct Sandwich<Board, Direction, rules::range::distance_1>
{
        template<typename T>
        T operator()(T from, T through, T dest) const
        {
                return (
                        Next<Board, Direction>()(from) &
                        through &
                        Prev<Board, Direction>()(dest)
                );
        }
};

template<typename Board, typename Direction>
struct Sandwich<Board, Direction, rules::range::distance_N>
{
        template<typename T>
        T operator()(T from, T through, T dest) const
        {
                return (
                        Next<Board, Direction>()(FloodFill<Board, Direction>()(from, dest)) &
                        through &
                        Prev<Board, Direction>()(dest)
                );
        }
};

}       // namespace dctl
