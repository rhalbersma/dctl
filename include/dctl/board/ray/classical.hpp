#pragma once
#include <dctl/board/angle.hpp>                 // Angle
#include <dctl/board/mask/king_targets.hpp>     // king_targets
#include <dctl/board/shift.hpp>                 // first, shift_sign
#include <dctl/board/ray/iterator.hpp>          // iterator
#include <dctl/utility/type_traits.hpp>         // set_t

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(iterator<Board, Direction> from, Set const propagator)
{
        constexpr auto theta = Angle{Direction};
        auto targets = mask::king_targets<Board>{}(*from, theta);
        auto const blockers = targets & ~propagator;
        if (blockers.any()) {
                auto const first = board::first<board::shift_sign_v<Direction>>{}(blockers);
                targets ^= mask::king_targets<Board>{}(first, theta);
                targets.reset(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
