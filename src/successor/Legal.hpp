#pragma once
#include "Driver_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "Selection.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Legal, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material, select::Jumps, Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Moves, Rules, Board> DoMoves;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoMoves::generate(p, moves);
        }

        template<typename Position>
        static int count(Position const& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoMoves::count(p);
                return num_moves;
        }

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
