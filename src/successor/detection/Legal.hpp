#pragma once
#include "../Driver_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../Selection.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors detection
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Legal, detection, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material, select::Jumps, detection, Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Moves, detection, Rules, Board> DoMoves;

public:

        template<typename Position>
        static bool detect(Position const& p)
        {
                // speculate #moves > #jumps so that the || is likely to short-circuit
                return (
                        DoMoves::detect(p) ||
                        DoJumps::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
