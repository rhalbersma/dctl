#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/all_jump.hpp>          // detect (piece jumps specialization)
#include <dctl/successor/detect/all_push.hpp>          // detect (piece moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>              // jumps
#include <dctl/successor/select/push.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, class Pieces, class Range>
struct detect<Color, Pieces, select::legal, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                using ShortMoves = detect<Color, Pieces, select::push, rules::range::distance_1>;
                using ShortJumps = detect<Color, Pieces, select::jump, rules::range::distance_1>;

                // speculate #moves > #jumps, so that the logical OR is more likely to short-circuit
                return ShortMoves{}(p) || ShortJumps{}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
