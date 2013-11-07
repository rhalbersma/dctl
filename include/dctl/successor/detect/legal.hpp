#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/all_jumps.hpp>        // detect (piece jumps specialization)
#include <dctl/successor/detect/all_moves.hpp>        // detect (piece moves specialization)
#include <dctl/successor/detect/king_jumps.hpp>         // detect (king jumps specialization)
#include <dctl/successor/detect/king_moves.hpp>         // detect (king moves specialization)
#include <dctl/successor/detect/pawn_jumps.hpp>         // detect (pawn jumps specialization)
#include <dctl/successor/detect/pawn_moves.hpp>         // detect (pawn moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/moves.hpp>              // moves

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
                using ShortMoves = detect<Color, Pieces, select::moves, rules::range::distance_1>;
                using ShortJumps = detect<Color, Pieces, select::jumps, rules::range::distance_1>;

                // speculate #moves > #jumps, so that the logical OR is more likely to short-circuit
                return ShortMoves{}(p) || ShortJumps{}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
