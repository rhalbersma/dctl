#pragma once
#include <dctl/successor/detection/detector_fwd.hpp>
#include <dctl/successor/detection/primary.hpp>
#include <dctl/successor/detection/both_jumps.hpp>
#include <dctl/successor/detection/both_moves.hpp>
#include <dctl/successor/detection/king_jumps.hpp>
#include <dctl/successor/detection/king_moves.hpp>
#include <dctl/successor/detection/pawn_jumps.hpp>
#include <dctl/successor/detection/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors detection
template<bool Color, int Material, typename Position, typename Range>
struct detector<Color, Material, Legal, Position, Range>
{
private:
        // typedefs

        // the detection of both pawn and king jumps and moves is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        typedef detector<Color, Material, Moves, Position, rules::range::distance_1> ShortMoves;

        // we do not have to test for the existence of long-ranged jumps because
        // because they imply the existence of short-ranged moves, for which we already test
        typedef detector<Color, Material, Jumps, Position, rules::range::distance_1> ShortJumps;

public:
        bool operator()(Position const& p) const
        {
                // speculate #moves > #jumps, so that the logical OR is more likely to short-circuit
                return ShortMoves()(p) || ShortJumps()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
