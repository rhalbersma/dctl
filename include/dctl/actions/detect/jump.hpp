#pragma once
#include <dctl/actions/detail/tracker.hpp>              // Tracker
#include <dctl/actions/detect/primary_fwd.hpp>          // Detect (primary template)
#include <dctl/actions/detect/detail/king_jump.hpp>     // Detect (king jump specialization)
#include <dctl/actions/detect/detail/pawn_jump.hpp>     // Detect (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <type_traits>                                  // true_type

namespace dctl {
namespace actions {

template<Color ToMove, class Reverse>
class Detect<ToMove, select::jump, Reverse>
{
public:
        template<class State>
        auto operator()(State const& state) const
        {
                using Tracker = detail::Tracker<ToMove, std::true_type, State>;
                using PawnJump = detail::Detect<ToMove, Piece::pawn, select::jump, Reverse, Tracker>;
                using KingJump = detail::Detect<ToMove, Piece::king, select::jump, Reverse, Tracker>;

                Tracker tracker{state};
                return
                        PawnJump{tracker}(state.pieces(ToMove, Piece::pawn)) ||
                        KingJump{tracker}(state.pieces(ToMove, Piece::king))
                ;
        }
};

}       // namespace actions
}       // namespace dctl
