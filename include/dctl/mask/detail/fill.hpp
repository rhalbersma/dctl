#pragma once
#include <dctl/board/angle.hpp>         // angle, is_positive
#include <dctl/utility/shift.hpp>       // shift

namespace dctl {
namespace mask {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<bool Sign, int N, class Set>
constexpr auto fill(Set generator, Set const propagator)
{
        Set flood {};
        while (generator.any()) {
                flood |= generator;
                generator = util::Shift<Sign>{}(generator, N) & propagator;
        }
        return flood;
}

}       // namespace loop

template<class Board, int Direction>
struct fill
{
        template<class Set>
        constexpr auto operator()(Set const generator, Set const propagator) const noexcept
        {
                constexpr auto theta = angle{Direction};
                return loop::fill<
                        is_positive(theta),
                        Board::shift_size(theta)
                >(generator, propagator);
        }
};

}       // namespace detail
}       // namespace mask
}       // namespace dctl
