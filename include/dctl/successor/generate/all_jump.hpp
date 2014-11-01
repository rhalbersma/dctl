#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // Generate (king jump specialization)
#include <dctl/successor/generate/pawn_jump.hpp>        // Generate (pawn jump specialization)
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <dctl/rule_traits.hpp>                         // is_absolute_king_jump_precedence_t, is_jump_precedence_t, is_remove_duplicates_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <algorithm>                                    // max_element, stable_sort, unique, upper_bound
#include <cassert>                                      // assert
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace successor {

// partial specialization for piece jumps
template<bool Color>
class Generate<Color, pieces::all, select::jump>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using Rules = rules_type_t<Position>;

                // EFFICIENCY: tag dispatching on absolute king jump precedence
                absolute_king_jump_precedence_dispatch(p, moves, is_absolute_king_jump_precedence_t<Rules>{});

                if (moves.size() <= 1)
                        return;

                filter_precedence_dispatch(moves, is_jump_precedence_t<Rules>{});
                filter_uniqueness_dispatch(moves, is_remove_duplicates_t<Rules>{});
        }

private:
        template<class Position, class Sequence>
        using KingJump = Generate<Color, pieces::king, select::jump, Position, Sequence>;

        template<class Position, class Sequence>
        using PawnJump = Generate<Color, pieces::pawn, select::jump, Position, Sequence>;

        // no absolute king jump precedence
        template<class Position, class Sequence>
        auto absolute_king_jump_precedence_dispatch(Position const& p, Sequence& moves, std::false_type) const
        {
                Tracker<Color, Position> tracker{p};
                KingJump<Position, Sequence>{tracker, moves}(p.kings(Color));
                PawnJump<Position, Sequence>{tracker, moves}(p.pawns(Color));
        }

        // absolute king jump precedence
        template<class Position, class Sequence>
        auto absolute_king_jump_precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                Tracker<Color, Position> tracker{p};
                KingJump<Position, Sequence>{tracker, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJump<Position, Sequence>{tracker, moves}(p.pawns(Color));
        }

        template<class Sequence>
        auto filter_precedence_dispatch(Sequence& /* moves */, std::false_type) const noexcept
        {
                // no-op
        }

        template<class Sequence>
        auto filter_precedence_dispatch(Sequence& moves, std::true_type) const
        {
                assert(!moves.empty()); // guarantees moves.front() exists

                using Move = typename Sequence::value_type;
                auto const greater = [](auto const& L, auto const& R){
                        return jump_precedence_t<rules_type_t<Move>>{}(R, L);
                };

                std::stable_sort(begin(moves), end(moves), greater);
                auto const drop = std::upper_bound(begin(moves), end(moves), moves.front(), greater);
                moves.erase(drop, end(moves));
        }

        template<class Sequence>
        void filter_uniqueness_dispatch(Sequence& /* moves */, std::false_type) const noexcept
        {
                // no-op
        }

        template<class Sequence>
        void filter_uniqueness_dispatch(Sequence& moves, std::true_type) const
        {
                assert(!moves.empty()); // guarantees me is dereferenceable

                auto const me = std::max_element(begin(moves), end(moves), [](auto const& L, auto const& R){
                        return L.num_pieces() < R.num_pieces();
                });

                using Move = typename Sequence::value_type;
                if (me->num_pieces() < large_jump_v<rules_type_t<Move>>)
                        return;

                std::stable_sort(begin(moves), end(moves));
                auto const drop = std::unique(begin(moves), end(moves));
                moves.erase(drop, end(moves));
        }
};

}       // namespace successor
}       // namespace dctl
