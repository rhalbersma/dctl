#pragma once
#include <dctl/actions/count/primary_fwd.hpp>           // Count (primary template)
#include <dctl/actions/count/detail/king_push.hpp>      // Count (king push specialization)
#include <dctl/actions/count/detail/pawn_push.hpp>      // Count (pawn push specialization)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Count<ToMove, select::push, Unique, Reverse>
{
public:
        template<class State>
        auto operator()(State const& state) const noexcept
        {
                using KingPush = detail::Count<ToMove, Piece::king, select::push, Reverse, State>;
                using PawnPush = detail::Count<ToMove, Piece::pawn, select::push, Reverse, State>;

                return KingPush{state}() + PawnPush{state}();
        }
};

}       // namespace actions
}       // namespace dctl
