#pragma once
#include "../board/Angle.hpp"
#include "../board/Traits.hpp"

namespace dctl {

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

template<typename Board, int Index>
struct FloodFill
{
        template<typename T>
        T operator()(T generator, T propagator) const
        {
                return board::flood_fill<
                        board::is_positive< angle<Index> >::value,
                        board::shift_size<Board, Index>::value
                >(generator, propagator);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, int Index>
struct Push
{
        template<typename T>
        T operator()(T square) const
        {
                return board::Shift<
                        board::is_positive< angle<Index> >::value,
                        board::shift_size<Board, Index>::value
                >()(square);
        }
};

// template function object for uniform left/right bitwise shift
template<typename Board, int Index>
struct Pull
{
        template<typename T>
        T operator()(T square) const
        {
                return board::Shift<
                        board::is_negative< angle<Index> >::value,
                        board::shift_size<Board, Index>::value
                >()(square);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PushAssign
{
        template<typename T>
        void operator()(T& square) const
        {
                board::ShiftAssign<
                        board::is_positive< angle<Index> >::value,
                        board::shift_size<Board, Index>::value
                >()(square);
        }
};

// template function object for uniform left/right bitwise shift-assignment
template<typename Board, int Index>
struct PullAssign
{
        template<typename T>
        void operator()(T& square) const
        {
                board::ShiftAssign<
                        board::is_negative< angle<Index> >::value,
                        board::shift_size<Board, Index>::value
                >()(square);
        }
};

namespace board {

template<typename Board, int Index>
struct shift_size;

template<typename Board> struct shift_size<Board, 0> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::right      ); };
template<typename Board> struct shift_size<Board, 1> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::right_up   ); };
template<typename Board> struct shift_size<Board, 2> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::up         ); };
template<typename Board> struct shift_size<Board, 3> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::left_up    ); };
template<typename Board> struct shift_size<Board, 4> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::left       ); };
template<typename Board> struct shift_size<Board, 5> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::left_down  ); };
template<typename Board> struct shift_size<Board, 6> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::down       ); };
template<typename Board> struct shift_size<Board, 7> { BOOST_STATIC_CONSTANT(auto, value = Board::InternalGrid::right_down ); };

// direction-wise flood-fill generator over propagator
template<bool Sign, size_t N, typename T>
T flood_fill(T generator, T propagator)
{
        return fill_loop<Sign, N>(generator, propagator);
}

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, size_t N, typename T>
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
template<bool, size_t>
struct Shift;

// partial specialization for bitwise shift-left
template<size_t N>
struct Shift<L, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return square << N;
        }
};

// partial specialization for bitwise shift-right
template<size_t N>
struct Shift<R, N>
{
        template<typename T>
        T operator()(T square) const
        {
                return square >> N;
        }
};

// primary template
template<bool, size_t>
struct ShiftAssign;

// partial specialization for bitwise shift-left assignment
template<size_t N>
struct ShiftAssign<L, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square <<= N;
        }
};

// partial specialization for bitwise shift-right assignment
template<size_t N>
struct ShiftAssign<R, N>
{
        template<typename T>
        void operator()(T& square) const
        {
                square >>= N;
        }
};

}       // namespace board
}       // namespace dctl
