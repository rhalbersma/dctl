#pragma once
#include <dctl/core/board/angle.hpp>                 // angle
#include <dctl/core/board/mask/king_targets.hpp>     // king_targets
#include <dctl/core/board/shift.hpp>                 // first, shift_sign
#include <dctl/core/board/ray/iterator.hpp>          // iterator
#include <dctl/util/type_traits.hpp>         // set_t

namespace dctl {
namespace core {
namespace board {
namespace ray {

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(iterator<Board, Direction> from, Set const propagator)
{
        constexpr auto theta = angle{Direction};
        auto targets = mask::king_targets<Board>{}(*from, theta);
        auto const blockers = targets & ~propagator;
        if (!blockers.empty()) {
                auto const first = board::first<board::shift_sign_v<Direction>>{}(blockers);
                targets ^= mask::king_targets<Board>{}(first, theta);
                targets.erase(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace board
}       // namespace core
}       // namespace dctl
