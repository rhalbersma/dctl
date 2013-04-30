#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/rules/traits.hpp>                        // distance_1

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, material::piece, select::moves, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, material::king, select::moves, Position, rules::range::distance_1> KingMoves;
                typedef impl::detect<Color, material::pawn, select::moves, Position, rules::range::distance_1> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(moveable_kings(p, Color)) || PawnMoves{propagate}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
