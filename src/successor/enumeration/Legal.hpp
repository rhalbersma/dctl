#pragma once
#include "../Driver_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../Result.hpp"
#include "../Select.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Legal, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material, select::Jumps, enumeration, Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Moves, enumeration, Rules, Board> DoMoves;

public:
        template<typename Position>
        static int count(Position const& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoMoves::count(p);
                return num_moves;
        }
};

}       // namespace successor
}       // namespace dctl
