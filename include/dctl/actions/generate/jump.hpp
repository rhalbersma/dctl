#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/actions/detail/filter.hpp>             // Precedence, Unique
#include <dctl/actions/detail/tracker.hpp>            // Tracker
#include <dctl/actions/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/generate/detail/king_jump.hpp> // Generate (king jump specialization)
#include <dctl/actions/generate/detail/pawn_jump.hpp> // Generate (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>               // jump

namespace dctl {
namespace actions {
namespace detail {

template<Player ToMove, bool Reverse>
class GenerateJump
{
public:
        template<class State, class Sequence>
        auto operator()(State const& p, Sequence& moves) const
        {
                using KingJump = detail::Generate<ToMove, Piece::king, select::jump, Reverse, State, Sequence>;
                using PawnJump = detail::Generate<ToMove, Piece::pawn, select::jump, Reverse, State, Sequence>;

                detail::Tracker<ToMove, State> tracker{p};
                KingJump{tracker, moves}(p.pieces(ToMove, Piece::king));
                PawnJump{tracker, moves}(p.pieces(ToMove, Piece::pawn));

                using rules_type = rules_type_t<State>;
                return filter::Precedence<rules_type>{}(begin(moves), end(moves));
        }
};

}       // namespace detail

template<Player ToMove, bool Reverse>
class Generate<ToMove, select::jump, false, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& p, Sequence& moves) const
        {
                auto it = detail::GenerateJump<ToMove, Reverse>{}(p, moves);
                moves.erase(it, end(moves));
        }
};

template<Player ToMove, bool Reverse>
class Generate<ToMove, select::jump, true, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& p, Sequence& moves) const
        {
                using rules_type = rules_type_t<State>;
                auto it1 = detail::GenerateJump<ToMove, Reverse>{}(p, moves);
                auto const it2 = filter::Unique<rules_type>{}(begin(moves), it1);
                moves.erase(it2, end(moves));
        }
};

}       // namespace actions
}       // namespace dctl
