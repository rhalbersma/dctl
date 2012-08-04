#pragma once
#include <functional>                   // function
#include "Detector_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors detection
template<bool Color, int Material, typename Position, typename Range>
struct detector<Color, Material, Legal, Position, Range>
:
        public std::function<bool(Position const&)>
{
private:
        // typedefs

        // the implementation of both pawn and king move detection is independent of Range, 
        // but we explicitly pass rules::range::distance_1 here to avoid code bloat
        typedef detector<Color, Material, Moves, Position, rules::range::distance_1> DoMoves;

        // because long-ranged jumps exist if and only if either short-ranged moves or 
        // short-ranged jumps exist, we can explicitly pass rules::range::distance_1 here
        typedef detector<Color, Material, Jumps, Position, rules::range::distance_1> DoJumps;

public:
        bool operator()(Position const& p) const
        {
                // speculate #moves > #jumps, so that the || is likely to short-circuit
                return (
                        DoMoves()(p) ||
                        DoJumps()(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
