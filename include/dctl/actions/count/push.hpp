#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/actions/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/actions/count/detail/king_push.hpp>    // Count (king push specialization)
#include <dctl/actions/count/detail/pawn_push.hpp>    // Count (pawn push specialization)
#include <dctl/actions/select/push.hpp>               // moves

namespace dctl {
namespace actions {

template<Player ToMove, bool RemoveDuplicates, bool Reverse>
class Count<ToMove, select::push, RemoveDuplicates, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using KingPush = detail::Count<ToMove, Piece::king, select::push, Reverse, State>;
                using PawnPush = detail::Count<ToMove, Piece::pawn, select::push, Reverse, State>;

                return KingPush{p}() + PawnPush{p}();
        }
};

}       // namespace actions
}       // namespace dctl
