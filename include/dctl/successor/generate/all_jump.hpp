#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // Generate (king jump specialization)
#include <dctl/successor/generate/pawn_jump.hpp>        // Generate (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/successor/filter.hpp>                    // Precedence, Unique

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Generate<ToMove, IsReverse, select::jump>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using KingJump = Generate<ToMove, IsReverse, PieceKingType, select::jump, Position, Sequence>;
                using PawnJump = Generate<ToMove, IsReverse, PiecePawnType, select::jump, Position, Sequence>;

                Tracker<ToMove, Position> tracker{p};
                KingJump{tracker, moves}(p.kings(ToMove));
                PawnJump{tracker, moves}(p.pawns(ToMove));

                using rules_type = rules_type_t<Position>;
                auto const it1 = filter::Precedence<rules_type>{}(begin(moves), end(moves));
                auto const it2 = filter::Unique<rules_type>{}(begin(moves), it1);
                moves.erase(it2, end(moves));
        }
};

}       // namespace successor
}       // namespace dctl
