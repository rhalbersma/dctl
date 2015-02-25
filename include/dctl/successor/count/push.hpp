#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/detail/king_push.hpp>    // Count (king push specialization)
#include <dctl/successor/count/detail/pawn_push.hpp>    // Count (pawn push specialization)
#include <dctl/successor/select/push.hpp>               // moves

namespace dctl {
namespace successor {

template<Color ToMove, bool RemoveDuplicates, bool Reverse>
class Count<ToMove, select::push, RemoveDuplicates, Reverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using KingPush = detail::Count<ToMove, Piece::king, select::push, Reverse, Position>;
                using PawnPush = detail::Count<ToMove, Piece::pawn, select::push, Reverse, Position>;

                return KingPush{p}() + PawnPush{p}();
        }
};

}       // namespace successor
}       // namespace dctl
