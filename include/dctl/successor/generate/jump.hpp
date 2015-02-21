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
namespace detail {

template<Color ToMove, bool Reverse>
class GenerateJump
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using KingJump = detail::Generate<ToMove, Piece::king, select::jump, Reverse, Position, Sequence>;
                using PawnJump = detail::Generate<ToMove, Piece::pawn, select::jump, Reverse, Position, Sequence>;

                detail::Tracker<ToMove, Position> tracker{p};
                KingJump{tracker, moves}(p.pieces(ToMove, Piece::king));
                PawnJump{tracker, moves}(p.pieces(ToMove, Piece::pawn));

                using rules_type = rules_type_t<Position>;
                return filter::Precedence<rules_type>{}(begin(moves), end(moves));
        }
};

}       // namespace detail

template<Color ToMove, bool Reverse>
class Generate<ToMove, select::jump, false, Reverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                auto it = detail::GenerateJump<ToMove, Reverse>{}(p, moves);
                moves.erase(it, end(moves));
        }
};

template<Color ToMove, bool Reverse>
class Generate<ToMove, select::jump, true, Reverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using rules_type = rules_type_t<Position>;
                auto it1 = detail::GenerateJump<ToMove, Reverse>{}(p, moves);
                auto const it2 = filter::Unique<rules_type>{}(begin(moves), it1);
                moves.erase(it2, end(moves));
        }
};

}       // namespace successor
}       // namespace dctl
