#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/king_push.hpp>           // Count (king push specialization)
#include <dctl/successor/count/pawn_push.hpp>           // Count (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // moves
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {

// partial specialization for piece moves
template<Color ToMove>
class Count<ToMove, pieces::all, select::push>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = Count<ToMove, pieces::king, select::push, Position>;
                using PawnPush = Count<ToMove, pieces::pawn, select::push, Position>;

                auto const propagate = p.not_occupied();
                return
                        KingPush{propagate}(moveable_kings(p, ToMove)) +
                        PawnPush{propagate}(p.pawns(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
