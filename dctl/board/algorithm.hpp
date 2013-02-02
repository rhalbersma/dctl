#pragma once
#include <dctl/angle/traits.hpp>        // is_positive
#include <dctl/board/shift.hpp>         // shift_size, Shift

namespace dctl {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<typename Sign, typename N, typename T>
T fill(T copy, T propagator)
{
        T flood(0);
        while (copy) {
                flood |= copy;
                copy = Shift<Sign, N>()(copy) & propagator;
        }
        return flood;
}

}       // namespace loop

// direction-wise flood-fill copy over propagator
template<typename Sign, typename N, typename T>
T fill(T copy, T propagator)
{
        return loop::fill<Sign, N>(copy, propagator);
}

}       // namespace detail

template<typename Board, typename Direction>
struct Fill
{
        template<typename T>
        T operator()(T copy, T propagator) const
        {
                return detail::fill< typename
                        angle::lazy::is_positive<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >(copy, propagator);
        }
};

}       // namespace dctl
