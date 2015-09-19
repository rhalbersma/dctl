#pragma once
#include <dctl/actions/detail/tracker.hpp>            // Tracker
#include <dctl/actions/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/generate/detail/king_jump.hpp> // Generate (king jump specialization)
#include <dctl/actions/generate/detail/pawn_jump.hpp> // Generate (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/color.hpp>                             // Color
#include <dctl/piece.hpp>                             // king, pawn

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Generate<ToMove, select::jump, Unique, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& s, Sequence& moves) const
        {
                using Tracker = detail::Tracker<ToMove, Unique, State>;
                using KingJump = detail::Generate<ToMove, Piece::king, select::jump, Reverse, Tracker, Sequence>;
                using PawnJump = detail::Generate<ToMove, Piece::pawn, select::jump, Reverse, Tracker, Sequence>;

                Tracker tracker{s};
                KingJump{tracker, moves}(s.pieces(ToMove, Piece::king));
                PawnJump{tracker, moves}(s.pieces(ToMove, Piece::pawn));
        }
};

}       // namespace actions
}       // namespace dctl
