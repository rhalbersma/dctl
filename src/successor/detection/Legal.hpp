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
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors detection
template<bool Color, int Material, typename Position>
struct detector<Color, Material, select::Legal, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef detector<Color, Material, select::Jumps, Position> DoJumps;
        typedef detector<Color, Material, select::Moves, Position> DoMoves;

public:
        static bool run(Position const& p)
        {
                // speculate #moves > #jumps so that the || is likely to short-circuit
                return (
                        DoMoves::run(p) ||
                        DoJumps::run(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
