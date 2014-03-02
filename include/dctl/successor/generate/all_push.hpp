#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/detail/king_push.hpp> // Generate (king push specialization)
#include <dctl/successor/generate/detail/pawn_push.hpp> // Generate (pawn push specialization)
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {

// partial specialization for piece moves
template<bool Color>
struct Generate<Color, pieces::all, select::push>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using KingPush = detail::Generate<Color, pieces::king, select::push, Position, Sequence>;
                using PawnPush = detail::Generate<Color, pieces::pawn, select::push, Position, Sequence>;

                Propagate<select::push, Position> const propagate{p};
                KingPush{propagate, moves}(moveable_kings(p, Color));
                PawnPush{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace successor
}       // namespace dctl
