#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_positive
#include <dctl/board/mask/king_targets.hpp>     // KingTargets
#include <dctl/board/ray/iterator.hpp>          // iterator
#include <dctl/utility/first.hpp>               // First
#include <dctl/utility/type_traits.hpp>         // set_t

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(iterator<Board, Direction> from, Set const& propagator)
{
        constexpr auto theta = angle{Direction};
        auto targets = KingTargets<Board>::mask(*from, theta);
        auto const blockers = targets & ~propagator;
        if (blockers.any()) {
                auto const first = util::First<is_positive(theta)>{}(blockers);
                targets ^= KingTargets<Board>::mask(first, theta);
                targets.reset(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
