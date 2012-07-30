#pragma once
#include "Enumerator_fwd.hpp"
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

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct enumerator<Color, Material, select::Legal, Position>
{
private:
        // typedefs

        typedef enumerator<Color, Material, select::Jumps, Position> DoJumps;
        typedef enumerator<Color, Material, select::Moves, Position> DoMoves;

public:
        int operator()(Position const& p)
        {
                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoMoves()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
