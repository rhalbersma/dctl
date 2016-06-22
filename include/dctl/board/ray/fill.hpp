#pragma once
#include <dctl/board/ray/iterator.hpp>  // iterator
#include <dctl/board/ray/traits.hpp>    // is_onboard

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction, class Set>
auto fill(iterator<Board, Direction> from, Set const propagator)
{
        Set targets {};
        for (++from; is_onboard(from) && propagator.test(*from); ++from)
                targets.set(*from);
        return targets;
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
