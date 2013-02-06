#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/both_jumps.hpp>         // detect (combined king and pawn jumps specialization)
#include <dctl/successor/detect/both_moves.hpp>         // detect (combined king and pawn moves specialization)
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

// partial specialization for legal successors detection
template<bool Color, int Material, typename Position, typename Range>
struct detect<Color, Material, select::legal, Position, Range>
{
        bool operator()(Position const& p) const
        {
                typedef detect<Color, Material, select::moves, Position, rules::range::distance_1> ShortMoves;
                typedef detect<Color, Material, select::jumps, Position, rules::range::distance_1> ShortJumps;

                // speculate #moves > #jumps, so that the logical OR is more likely to short-circuit
                return ShortMoves(p) || ShortJumps(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
