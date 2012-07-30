#pragma once
#include "Generator_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Legal, Position>
{
        void operator()(Position const& p, Stack& moves)
        {
                generator<Color, Material, Jumps, Position>()(p, moves);
                if (moves.empty())
                        generator<Color, Material, Moves, Position>()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
