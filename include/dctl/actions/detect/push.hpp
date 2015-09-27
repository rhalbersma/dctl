#pragma once
#include <dctl/actions/detect/primary_fwd.hpp>          // Detect (primary template)
#include <dctl/actions/detect/detail/king_push.hpp>     // Detect (king push specialization)
#include <dctl/actions/detect/detail/pawn_push.hpp>     // Detect (pawn push specialization)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <type_traits>                                  // false_type

namespace dctl {
namespace actions {

template<Color ToMove, class Reverse>
class Detect<ToMove, select::push, Reverse>
{
public:
        template<class State>
        auto operator()(State const& state) const noexcept
        {
                using PawnPush = detail::Detect<ToMove, Piece::pawn, select::push, Reverse, State>;
                using KingPush = detail::Detect<ToMove, Piece::king, select::push, Reverse, State>;

                return PawnPush{state}() || KingPush{state}();
        }
};

}       // namespace actions
}       // namespace dctl
