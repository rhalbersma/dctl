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
#include <iterator>
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

                if (moves.size() < 2)
                        return;

                auto drop1 = filter_precedence_dispatch(begin(moves), end(moves), is_jump_precedence_t<Rules>{});
                auto drop2 = filter_uniqueness_dispatch(begin(moves), drop1, is_remove_duplicates_t<Rules>{});
                moves.erase(drop2, end(moves));
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

        template<class It>
        auto filter_precedence_dispatch(It /* first */, It last, std::false_type) const noexcept
        {
                return last;
        }

        template<class It>
        auto filter_precedence_dispatch(It first, It last, std::true_type) const
        {
                assert(1 <= std::distance(first, last)); // guarantees first is dereferenceable

                using Move = typename It::value_type;
                auto const greater = [](auto const& L, auto const& R){
                        return jump_precedence_t<rules_type_t<Move>>{}(R, L);
                };

                std::stable_sort(first, last, greater);
                return std::upper_bound(first, last, *first, greater);
        }

        template<class It>
        auto filter_uniqueness_dispatch(It /* first */, It last, std::false_type) const noexcept
        {
                return last;
        }

        template<class It>
        auto filter_uniqueness_dispatch(It first, It last, std::true_type) const
        {
                assert(1 <= std::distance(first, last)); // guarantees me is dereferenceable
                if (std::distance(first, last) == 1)
                        return last;

                auto const me = std::max_element(first, last, [](auto const& L, auto const& R){
                        return L.num_pieces() < R.num_pieces();
                });

                using Move = typename It::value_type;
                if (me->num_pieces() < large_jump_v<rules_type_t<Move>>)
                        return last;

                std::stable_sort(first, last);
                return std::unique(first, last);
        }
};

}       // namespace successor
}       // namespace dctl
