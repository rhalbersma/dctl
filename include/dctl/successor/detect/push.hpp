#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_push.hpp>          // Detect (king push specialization)
#include <dctl/successor/detect/pawn_push.hpp>          // Detect (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // push
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Detect<ToMove, select::push, IsReverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using PawnPush = Detect<ToMove, select::push, IsReverse, PiecePawnType, Position>;
                using KingPush = Detect<ToMove, select::push, IsReverse, PieceKingType, Position>;
                using rules_type = rules_type_t<Position>;

                auto const not_occupied = p.not_occupied();
                return
                        PawnPush{not_occupied}(p.pieces(ToMove, Piece::pawn)) ||
                        KingPush{not_occupied}(p.pieces(ToMove, PieceKingType{}, is_restricted_king_push_t<rules_type>{}))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
