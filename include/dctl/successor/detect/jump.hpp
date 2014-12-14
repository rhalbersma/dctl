#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/successor/detail/tracker.hpp>            // Tracker
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_jump.hpp>          // Detect (king jump specialization)
#include <dctl/successor/detect/pawn_jump.hpp>          // Detect (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Detect<ToMove, select::jump, IsReverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using PawnJump = Detect<ToMove, select::jump, IsReverse, PiecePawnType, Position>;
                using KingJump = Detect<ToMove, select::jump, IsReverse, PieceKingType, Position>;

                detail::Tracker<ToMove, Position> tracker{p};
                return
                        PawnJump{tracker}(p.pawns(ToMove)) ||
                        KingJump{tracker}(p.kings(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
