#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/successor/detail/tracker.hpp>            // Tracker
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/detail/king_jump.hpp>   // Detect (king jump specialization)
#include <dctl/successor/detect/detail/pawn_jump.hpp>   // Detect (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump

namespace dctl {
namespace successor {

template<Color ToMove, bool Reverse>
class Detect<ToMove, select::jump, Reverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using PawnJump = detail::Detect<ToMove, Piece::pawn, select::jump, Reverse, Position>;
                using KingJump = detail::Detect<ToMove, Piece::king, select::jump, Reverse, Position>;

                detail::Tracker<ToMove, Position> tracker{p};
                return
                        PawnJump{tracker}(p.pieces(ToMove, Piece::pawn)) ||
                        KingJump{tracker}(p.pieces(ToMove, Piece::king))
                ;
        }
};

}       // namespace successor
}       // namespace dctl