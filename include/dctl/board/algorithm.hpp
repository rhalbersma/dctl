#pragma once
#include <dctl/angle/traits.hpp>        // is_positive
#include <dctl/board/shift.hpp>         // shift_size, Shift

namespace dctl {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<class Sign, class N, class T>
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
template<class Sign, class N, class T>
T fill(T copy, T propagator)
{
        return loop::fill<Sign, N>(copy, propagator);
}

}       // namespace detail

template<class Board, class Direction>
struct Fill
{
        template<class T>
        T operator()(T copy, T propagator) const
        {
                return detail::fill<
                        boost::mpl::bool_< angle::is_positive(Direction::value) >,
                        shift_size<Board, Direction>
                >(copy, propagator);
        }
};

}       // namespace dctl
