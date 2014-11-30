#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_push.hpp>          // Detect (king push specialization)
#include <dctl/successor/detect/pawn_push.hpp>          // Detect (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<Color ToMove, class Range>
class Detect<ToMove, pieces::all, select::push, Range>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = Detect<ToMove, pieces::king, select::push, Position, std::false_type>;
                using PawnPush = Detect<ToMove, pieces::pawn, select::push, Position, std::false_type>;

                auto const propagate = p.not_occupied();

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        KingPush{propagate}(moveable_kings(p, ToMove)) ||
                        PawnPush{propagate}(p.pawns(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
