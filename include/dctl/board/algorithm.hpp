#pragma once
#include <dctl/board/angle.hpp>         // angle, is_positive
#include <dctl/utility/shift.hpp>       // shift

namespace dctl {
namespace board {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, int N, class Set>
auto fill(Set generator, Set const& propagator)
{
        Set flood {};
        while (generator.any()) {
                flood |= generator;
                generator = util::Shift<Sign>{}(generator, N) & propagator;
        }
        return flood;
}

}       // namespace loop

// direction-wise flood-fill copy over propagator
template<bool Sign, int N, class Set>
auto fill(Set const& generator, Set const& propagator)
{
        return loop::fill<Sign, N>(generator, propagator);
}

}       // namespace detail

template<class Board, int Direction>
struct Fill
{
        template<class Set>
        auto operator()(Set const& generator, Set const& propagator) const
        {
                constexpr auto theta = angle{Direction};
                return detail::fill<
                        is_positive(theta),
                        Board::shift_size(theta)
                >(generator, propagator);
        }
};

}       // namespace board
}       // namespace dctl
