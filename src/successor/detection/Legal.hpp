#pragma once
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
template<bool Color, int Material, typename Position>
struct detector<Color, Material, Legal, Position>
{
private:
        // typedefs

        typedef detector<Color, Material, Jumps, Position> DoJumps;
        typedef detector<Color, Material, Moves, Position> DoMoves;

public:
        bool operator()(Position const& p)
        {
                // speculate #moves > #jumps so that the || is likely to short-circuit
                return (
                        DoMoves()(p) ||
                        DoJumps()(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
