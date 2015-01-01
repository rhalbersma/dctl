#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/successor/detail/filter.hpp>             // Precedence, Unique
#include <dctl/successor/detail/tracker.hpp>            // Tracker
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/detail/king_jump.hpp> // Generate (king jump specialization)
#include <dctl/successor/generate/detail/pawn_jump.hpp> // Generate (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Generate<ToMove, select::jump, IsReverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using KingJump = detail::Generate<ToMove, Piece::king, select::jump, IsReverse, Position, Sequence>;
                using PawnJump = detail::Generate<ToMove, Piece::pawn, select::jump, IsReverse, Position, Sequence>;

                detail::Tracker<ToMove, Position> tracker{p};
                KingJump{tracker, moves}(p.pieces(ToMove, Piece::king));
                PawnJump{tracker, moves}(p.pieces(ToMove, Piece::pawn));

                using rules_type = rules_type_t<Position>;
                auto const it1 = filter::Precedence<rules_type>{}(begin(moves), end(moves));
                auto const it2 = filter::Unique<rules_type>{}(begin(moves), it1);
                moves.erase(it2, end(moves));
        }
};

}       // namespace successor
}       // namespace dctl
