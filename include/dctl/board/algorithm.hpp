#pragma once
#include <dctl/angle/traits.hpp>        // is_positive
#include <dctl/board/shift.hpp>         // shift_size, Shift

namespace dctl {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, class N, class T>
T fill(T generator, T propagator)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, N::value) & propagator;
        }
        return flood;
}

}       // namespace loop

// direction-wise flood-fill copy over propagator
template<bool Sign, class N, class T>
T fill(T generator, T propagator)
{
        return loop::fill<Sign, N>(generator, propagator);
}

}       // namespace detail

template<class Board, int Direction>
struct Fill
{
        template<class T>
        T operator()(T generator, T propagator) const
        {
                return detail::fill<
                        angle::is_positive(Direction),
                        board::shift_size<Board, Direction>
                >(generator, propagator);
        }
};

}       // namespace dctl
