#pragma once
#include <dctl/angle/angle.hpp>
#include <dctl/angle/traits.hpp>        // is_positive
#include <dctl/utility/shift.hpp>       // shift

namespace dctl {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, int N, class T>
T fill(T generator, T propagator)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = util::shift<Sign>()(generator, N) & propagator;
        }
        return flood;
}

}       // namespace loop

// direction-wise flood-fill copy over propagator
template<bool Sign, int N, class T>
T fill(T generator, T propagator)
{
        return loop::fill<Sign, N>(generator, propagator);
}

}       // namespace detail

template<class Board, int Direction>
struct Fill
{
        static constexpr auto theta = Angle{Direction};

        template<class T>
        T operator()(T generator, T propagator) const
        {
                return detail::fill<
                        is_positive(theta),
                        Board::shift_size(theta)
                >(generator, propagator);
        }
};

}       // namespace dctl
