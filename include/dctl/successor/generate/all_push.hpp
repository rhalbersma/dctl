#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_push.hpp>        // Generate (king push specialization)
#include <dctl/successor/generate/pawn_push.hpp>        // Generate (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {

// partial specialization for piece moves
template<bool Color>
class Generate<Color, pieces::all, select::push>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using KingPush = Generate<Color, pieces::king, select::push, Position, Sequence>;
                using PawnPush = Generate<Color, pieces::pawn, select::push, Position, Sequence>;

                auto const propagate = p.not_occupied();
                KingPush{propagate, moves}(moveable_kings(p, Color));
                PawnPush{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace successor
}       // namespace dctl
