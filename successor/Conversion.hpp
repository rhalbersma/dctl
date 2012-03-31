#pragma once
#include "Driver_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothPromotion.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "PawnPromotion.hpp"
#include "Selection.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/Stack.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Conversion, Rules, Board>
:
        private nonconstructible        // enforce static semantics
{
private:
        // typedefs
        typedef Driver<Color, Material, select::Jumps     , Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Promotions, Rules, Board> DoPromotions;

public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoPromotions::generate(p, moves);
        }

        static int count(const Position<Board>& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoPromotions::count(p);
                return num_moves;
        }

        static bool detect(const Position<Board>& p)
        {
                return (
                        DoJumps::detect(p) ||
                        DoPromotions::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
