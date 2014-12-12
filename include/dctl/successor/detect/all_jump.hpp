#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_jump.hpp>          // Detect (king jump specialization)
#include <dctl/successor/detect/pawn_jump.hpp>          // Detect (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Detect<ToMove, IsReverse, select::jump>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using PawnJump = Detect<ToMove, IsReverse, PiecePawnType, select::jump, Position>;
                using KingJump = Detect<ToMove, IsReverse, PieceKingType, select::jump, Position>;

                Tracker<ToMove, Position> tracker{p};

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                // SPECULATE: #pawn jumps > #king jumps for earliest possible short-circuiting
                return
                        PawnJump{tracker}(p.pawns(ToMove)) ||
                        KingJump{tracker}(p.kings(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
