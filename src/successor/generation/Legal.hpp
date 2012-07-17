#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors generation
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Legal, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material, select::Jumps, generation, Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Moves, generation, Rules, Board> DoMoves;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoMoves::generate(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
