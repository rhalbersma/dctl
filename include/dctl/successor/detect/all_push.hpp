#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_push.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_push.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/propagate/push.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/push.hpp>              // moves
#include <dctl/position/unary_projections.hpp>              // moveable_kings
#include <dctl/rules/traits.hpp>                        // distance_1
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, class Range>
struct detect<Color, pieces::all, select::push, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                using KingMoves = impl::detect<Color, pieces::king, select::push, Position, rules::range::distance_1>;
                using PawnMoves = impl::detect<Color, pieces::pawn, select::push, Position, rules::range::distance_1>;

                Propagate<select::push, Position> const propagate(p);
                return
                        KingMoves{propagate}(moveable_kings(p, Color)) ||
                        PawnMoves{propagate}(p.pawns(Color))
                ;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
