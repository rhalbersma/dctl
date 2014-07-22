#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_push.hpp>          // Detect (king push specialization)
#include <dctl/successor/detect/pawn_push.hpp>          // Detect (pawn push specialization)
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<bool Color, class Range>
class Detect<Color, pieces::all, select::push, Range>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = Detect<Color, pieces::king, select::push, Position, std::false_type>;
                using PawnPush = Detect<Color, pieces::pawn, select::push, Position, std::false_type>;

                auto const propagate = Propagate<select::push, Position>{p};

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        KingPush{propagate}(moveable_kings(p, Color)) ||
                        PawnPush{propagate}(p.pawns(Color))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
