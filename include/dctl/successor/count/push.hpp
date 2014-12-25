#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/king_push.hpp>           // Count (king push specialization)
#include <dctl/successor/count/pawn_push.hpp>           // Count (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // moves

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Count<ToMove, select::push, IsReverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = Count<ToMove, select::push, IsReverse, PieceKingType, Position>;
                using PawnPush = Count<ToMove, select::push, IsReverse, PiecePawnType, Position>;
                using rules_type = rules_type_t<Position>;

                auto const not_occupied = p.not_occupied();
                return
                        KingPush{not_occupied}(p.pieces(ToMove, PieceKingType{}, is_restricted_king_push_t<rules_type>{})) +
                        PawnPush{not_occupied}(p.pieces(ToMove, Piece::pawn))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
