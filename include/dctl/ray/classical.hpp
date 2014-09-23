#pragma once
#include <dctl/angle.hpp>                       // Angle, is_positive
#include <dctl/board/mask/king_targets.hpp>     // KingTargets
#include <dctl/board/mask/squares.hpp>          // Squares
#include <dctl/ray/iterator.hpp>                // Iterator
#include <dctl/type_traits.hpp>
#include <dctl/utility/first.hpp>               // First
#include <iterator>                             // next

namespace dctl {
namespace ray {

template<class Board, int Direction, class Set = set_type_t<Board>>
auto classical(ray::Iterator<Board, Direction> from, Set const& propagator)
{
        auto targets = board::KingTargets<Board>::mask(Angle{Direction}, *from);
        auto const blocker = targets & (board::Squares<Board>::mask() ^ propagator);
        if (blocker.any()) {
                auto const first = util::First<is_positive(Angle{Direction})>{}(blocker);
                targets ^= board::KingTargets<Board>::mask(Angle{Direction}, first);
                targets.reset(first);
        }
        return targets;
}

}       // namespace ray
}       // namespace dctl
