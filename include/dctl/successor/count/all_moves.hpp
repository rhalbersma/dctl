#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/king_moves.hpp>     // count (king moves specialization)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/pieces/pieces.hpp>                // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece moves
template<bool Color>
struct count<Color, pieces::all, select::moves>
{
        template<class Position>
        int operator()(Position const& p) const
        {
                using KingMoves = impl::count<Color, pieces::king, select::moves, Position>;
                using PawnMoves = impl::count<Color, pieces::pawn, select::moves, Position>;

                Propagate<select::moves, Position> const propagate(p);
                return
                        KingMoves{propagate}(moveable_kings(p, Color)) +
                        PawnMoves{propagate}(p.material().pawns(Color))
                ;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
