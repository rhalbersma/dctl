#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/detail/king_push.hpp>   // Detect (king push specialization)
#include <dctl/successor/detect/detail/pawn_push.hpp>   // Detect (pawn push specialization)
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
                using PawnPush = detail::Detect<ToMove, Piece::pawn, select::push, IsReverse, Position>;
                using KingPush = detail::Detect<ToMove, Piece::king, select::push, IsReverse, Position>;

                return PawnPush{p}() || KingPush{p}();
        }
};

}       // namespace successor
}       // namespace dctl
