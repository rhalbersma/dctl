#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_push.hpp>        // Generate (king push specialization)
#include <dctl/successor/generate/pawn_push.hpp>        // Generate (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Generate<ToMove, select::push, IsReverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using KingPush = Generate<ToMove, select::push, IsReverse, PieceKingType, Position, Sequence>;
                using PawnPush = Generate<ToMove, select::push, IsReverse, PiecePawnType, Position, Sequence>;
                using rules_type = rules_type_t<Position>;

                auto const not_occupied = p.not_occupied();
                KingPush{not_occupied, moves}(p.pieces(ToMove, PieceKingType{}, is_restricted_king_push_t<rules_type>{}));
                PawnPush{not_occupied, moves}(p.pieces(ToMove, Piece::pawn));
        }
};

}       // namespace successor
}       // namespace dctl
