#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/successor/copy/both_moves.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/copy/king_moves.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/copy/pawn_moves.hpp>
#include <dctl/successor/select/legal.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct copy<Color, Material, select::legal, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef copy<Color, Material, select::jumps, Position> DoJumps;
                typedef copy<Color, Material, select::moves, Position> DoMoves;

                DoJumps()(p, moves);
                if (moves.empty())
                        DoMoves()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
