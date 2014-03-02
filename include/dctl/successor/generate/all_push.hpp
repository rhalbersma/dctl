#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // generate (primary template)
#include <dctl/successor/generate/impl/king_push.hpp>  // generate (king moves specialization)
#include <dctl/successor/generate/impl/pawn_push.hpp>  // generate (pawn moves specialization)
#include <dctl/successor/propagate/push.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/push.hpp>              // moves
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece moves
template<bool Color>
struct generate<Color, pieces::all, select::push>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using KingMoves = impl::generate<Color, pieces::king, select::push, Position, Sequence>;
                using PawnMoves = impl::generate<Color, pieces::pawn, select::push, Position, Sequence>;

                Propagate<select::push, Position> const propagate{p};
                KingMoves{propagate, moves}(moveable_kings(p, Color));
                PawnMoves{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
