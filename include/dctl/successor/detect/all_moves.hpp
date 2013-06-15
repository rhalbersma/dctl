#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/rules/traits.hpp>                        // distance_1
#include <dctl/pieces/pieces.hpp>                // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, class Range>
struct detect<Color, pieces::all, select::moves, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, pieces::king, select::moves, Position, rules::range::distance_1> KingMoves;
                typedef impl::detect<Color, pieces::pawn, select::moves, Position, rules::range::distance_1> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(moveable_kings(p, Color)) || PawnMoves{propagate}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
