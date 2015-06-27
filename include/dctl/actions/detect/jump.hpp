#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/actions/detail/tracker.hpp>            // Tracker
#include <dctl/actions/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/actions/detect/detail/king_jump.hpp>   // Detect (king jump specialization)
#include <dctl/actions/detect/detail/pawn_jump.hpp>   // Detect (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>               // jump

namespace dctl {
namespace actions {

template<Player ToMove, bool Reverse>
class Detect<ToMove, select::jump, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using PawnJump = detail::Detect<ToMove, Piece::pawn, select::jump, Reverse, State>;
                using KingJump = detail::Detect<ToMove, Piece::king, select::jump, Reverse, State>;

                detail::Tracker<ToMove, State> tracker{p};
                return
                        PawnJump{tracker}(p.pieces(ToMove, Piece::pawn)) ||
                        KingJump{tracker}(p.pieces(ToMove, Piece::king))
                ;
        }
};

}       // namespace actions
}       // namespace dctl
