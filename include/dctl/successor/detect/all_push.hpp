#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/detail/king_push.hpp>   // Detect (king push specialization)
#include <dctl/successor/detect/detail/pawn_push.hpp>   // Detect (pawn push specialization)
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/rules/traits.hpp>                        // distance_1
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {

template<bool Color, class Range>
struct Detect<Color, pieces::all, select::push, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                using KingPush = detail::Detect<Color, pieces::king, select::push, Position, rules::range::distance_1>;
                using PawnPush = detail::Detect<Color, pieces::pawn, select::push, Position, rules::range::distance_1>;

                Propagate<select::push, Position> const propagate(p);
                return
                        KingPush{propagate}(moveable_kings(p, Color)) ||
                        PawnPush{propagate}(p.pawns(Color))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
