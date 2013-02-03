#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>  // copy (primary template)
#include <dctl/successor/copy/both_jumps.hpp>   // copy (combined king and pawn jumps specialization)
#include <dctl/successor/copy/both_moves.hpp>   // copy (combined king and pawn moves specialization)
#include <dctl/successor/copy/king_jumps.hpp>   // copy (king jumps specialization)
#include <dctl/successor/copy/king_moves.hpp>   // copy (king moves specialization)
#include <dctl/successor/copy/pawn_jumps.hpp>   // copy (pawn jumps specialization)
#include <dctl/successor/copy/pawn_moves.hpp>   // copy (pawn moves specialization)
#include <dctl/successor/select/legal.hpp>      // legal
#include <dctl/successor/select/jumps.hpp>      // jumps
#include <dctl/successor/select/moves.hpp>      // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct copy<Color, Material, select::legal, Position>
{
        template<typename Vector>
        void operator()(Position const& p, Vector& moves) const
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
