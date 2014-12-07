#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/position/unary_projections.hpp>          // moveable_kings
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/king_push.hpp>           // Count (king push specialization)
#include <dctl/successor/count/pawn_push.hpp>           // Count (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // moves

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Count<ToMove, IsReverse, select::push>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = Count<ToMove, IsReverse, PieceKingType, select::push, Position>;
                using PawnPush = Count<ToMove, IsReverse, PiecePawnType, select::push, Position>;

                auto const not_occupied = p.not_occupied();
                return
                        KingPush{not_occupied}(moveable_kings(p, ToMove)) +
                        PawnPush{not_occupied}(p.pawns(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
