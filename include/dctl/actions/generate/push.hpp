#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/actions/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/generate/detail/king_push.hpp> // Generate (king push specialization)
#include <dctl/actions/generate/detail/pawn_push.hpp> // Generate (pawn push specialization)
#include <dctl/actions/select/push.hpp>               // push

namespace dctl {
namespace actions {

template<Player ToMove, bool RemoveDuplicates, bool Reverse>
class Generate<ToMove, select::push, RemoveDuplicates, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& p, Sequence& moves) const
        {
                using KingPush = detail::Generate<ToMove, Piece::king, select::push, Reverse, State, Sequence>;
                using PawnPush = detail::Generate<ToMove, Piece::pawn, select::push, Reverse, State, Sequence>;

                KingPush{p, moves}();
                PawnPush{p, moves}();
        }
};

}       // namespace actions
}       // namespace dctl
