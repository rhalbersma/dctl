#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/detail/king_push.hpp> // Generate (king push specialization)
#include <dctl/successor/generate/detail/pawn_push.hpp> // Generate (pawn push specialization)
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
                using KingPush = detail::Generate<ToMove, Piece::king, select::push, IsReverse, Position, Sequence>;
                using PawnPush = detail::Generate<ToMove, Piece::pawn, select::push, IsReverse, Position, Sequence>;

                KingPush{p, moves}();
                PawnPush{p, moves}();
        }
};

}       // namespace successor
}       // namespace dctl
