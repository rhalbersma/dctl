#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/detail/king_push.hpp>    // Count (king push specialization)
#include <dctl/successor/count/detail/pawn_push.hpp>    // Count (pawn push specialization)
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>               // moves
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {

// partial specialization for piece moves
template<bool Color>
struct Count<Color, pieces::all, select::push>
{
        template<class Position>
        int operator()(Position const& p) const
        {
                using KingPush = detail::Count<Color, pieces::king, select::push, Position>;
                using PawnPush = detail::Count<Color, pieces::pawn, select::push, Position>;

                Propagate<select::push, Position> const propagate(p);
                return
                        KingPush{propagate}(moveable_kings(p, Color)) +
                        PawnPush{propagate}(p.pawns(Color))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
