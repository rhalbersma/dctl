#pragma once
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_restricted_king_push_t
#include <dctl/actions/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/actions/detect/detail/king_push.hpp>   // Detect (king push specialization)
#include <dctl/actions/detect/detail/pawn_push.hpp>   // Detect (pawn push specialization)
#include <dctl/actions/select/push.hpp>               // push
#include <type_traits>                                  // false_type

namespace dctl {
namespace actions {

template<Color ToMove, bool Reverse>
class Detect<ToMove, select::push, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using PawnPush = detail::Detect<ToMove, Piece::pawn, select::push, Reverse, State>;
                using KingPush = detail::Detect<ToMove, Piece::king, select::push, Reverse, State>;

                return PawnPush{p}() || KingPush{p}();
        }
};

}       // namespace actions
}       // namespace dctl
