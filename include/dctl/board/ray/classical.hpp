#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_positive
#include <dctl/board/mask/king_targets.hpp>     // KingTargets
#include <dctl/board/ray/iterator.hpp>          // Iterator
#include <dctl/utility/first.hpp>               // First
#include <dctl/utility/type_traits.hpp>         // set_t

namespace dctl {
namespace ray {

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(ray::Iterator<Board, Direction> from, Set const& propagator)
{
        constexpr auto theta = angle{Direction};
        auto targets = board::KingTargets<Board>::mask(theta, *from);
        auto const blockers = targets & ~propagator;
        if (blockers.any()) {
                auto const first = util::First<is_positive(theta)>{}(blockers);
                targets ^= board::KingTargets<Board>::mask(theta, first);
                targets.reset(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace dctl
