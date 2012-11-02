#pragma once
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/traits.hpp>
#include <dctl/rules/enum.hpp>

namespace dctl {

// TODO: use C+11 template aliases
template<typename Board, typename Direction>
struct jump_start
:
        Board::template jump_start<Direction>
{};

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

}       // namespace dctl
