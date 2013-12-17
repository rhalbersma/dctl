#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // generate (primary template)
#include <dctl/successor/generate/impl/king_moves.hpp>  // generate (king moves specialization)
#include <dctl/successor/generate/impl/pawn_moves.hpp>  // generate (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece moves
template<bool Color>
struct generate<Color, pieces::all, select::moves>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using KingMoves = impl::generate<Color, pieces::king, select::moves, Position, Sequence>;
                using PawnMoves = impl::generate<Color, pieces::pawn, select::moves, Position, Sequence>;

                Propagate<select::moves, Position> const propagate(p);
                KingMoves{propagate, moves}(moveable_kings(p, Color));
                PawnMoves{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
