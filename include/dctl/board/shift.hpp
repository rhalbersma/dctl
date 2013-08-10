#pragma once
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/traits.hpp>

namespace dctl {

template<class Board, class Direction>
using jump_start = typename Board::template jump_start<Direction>::type;

template<class Board, class Direction>
using shift_size = typename Board::template shift_size<Direction>::type;

// left and right direction

using L = boost::mpl::false_;
using R = boost::mpl::true_;

// primary template
template<class, class>
struct Shift;

// partial specialization for bitwise shift-left
template<class N>
struct Shift<L, N>
{
        template<class T>
        T operator()(T square) const
        {
                return square << N::value;
        }
};

// partial specialization for bitwise shift-right
template<class N>
struct Shift<R, N>
{
        template<class T>
        T operator()(T square) const
        {
                return square >> N::value;
        }
};

// primary template
template<class, class>
struct ShiftAssign;

// partial specialization for bitwise shift-left assignment
template<class N>
struct ShiftAssign<L, N>
{
        template<class T>
        void operator()(T& square) const
        {
                square <<= N::value;
        }
};

// partial specialization for bitwise shift-right assignment
template<class N>
struct ShiftAssign<R, N>
{
        template<class T>
        void operator()(T& square) const
        {
                square >>= N::value;
        }
};

}       // namespace dctl
