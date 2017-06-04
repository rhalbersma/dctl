#pragma once
#include <dctl/core/board/shift.hpp> // shift, shift_sign, shift_size

namespace dctl {
namespace core {
namespace board {
namespace mask {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<board::direction Direction, int N, class Set>
constexpr auto fill(Set generator, Set const propagator)
{
        Set flood {};
        while (!generator.empty()) {
                flood |= generator;
                generator = board::shift<Direction, N>{}(generator) & propagator;
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
                        board::shift_sign_v<Direction>,
                        board::shift_size_v<Board, Direction>
                >(generator, propagator);
        }
};

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace core
}       // namespace dctl
