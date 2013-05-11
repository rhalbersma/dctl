#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/king_moves.hpp>     // count (king moves specialization)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece moves
template<bool Color>
struct count<Color, material::piece, select::moves>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                typedef impl::count<Color, material::king, select::moves, Position> KingMoves;
                typedef impl::count<Color, material::pawn, select::moves, Position> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(moveable_kings(p, Color)) + PawnMoves{propagate}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
