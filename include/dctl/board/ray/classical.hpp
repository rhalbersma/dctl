#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_positive
#include <dctl/mask/king_targets.hpp>     // king_targets
#include <dctl/board/ray/iterator.hpp>          // iterator
#include <dctl/utility/first.hpp>               // First
#include <dctl/utility/type_traits.hpp>         // set_t

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(iterator<Board, Direction> from, Set const propagator)
{
        constexpr auto theta = angle{Direction};
        auto targets = mask::king_targets<Board>{}(*from, theta);
        auto const blockers = targets & ~propagator;
        if (blockers.any()) {
                auto const first = util::First<is_positive(theta)>{}(blockers);
                targets ^= mask::king_targets<Board>{}(first, theta);
                targets.reset(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
