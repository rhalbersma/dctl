#pragma once
#include <dctl/core/board/detail/shift.hpp>    // shift, shift_sign, shift_size

namespace dctl::core {
namespace mask {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<detail::direction Direction, int N, class Set>
constexpr auto fill(Set generator, Set const propagator)
{
        Set flood {};
        while (not generator.empty()) {
                flood |= generator;
                generator = detail::shift<Direction, N>{}(generator) & propagator;
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
                return loop::fill<
                        detail::shift_sign_v<Direction>,
                        detail::shift_size_v<Board, Direction>
                >(generator, propagator);
        }
};

}       // namespace mask
}       // namespace dctl::core
