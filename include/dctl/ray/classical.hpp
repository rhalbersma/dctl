#pragma once
#include <dctl/angle.hpp>                       // Angle, is_positive
#include <dctl/board/mask/king_targets.hpp>     // KingTargets
#include <dctl/ray/iterator.hpp>                // Iterator
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/utility/first.hpp>               // First

namespace dctl {
namespace ray {

template<class Board, int Direction, class Set = get_set_type<Board>>
auto classical(ray::Iterator<Board, Direction> from, Set const& propagator)
{
        constexpr auto theta = Angle{Direction};
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
