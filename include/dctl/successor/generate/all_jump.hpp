#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // Generate (king jump specialization)
#include <dctl/successor/generate/pawn_jump.hpp>        // Generate (pawn jump specialization)
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <dctl/rule_traits.hpp>                         // is_absolute_king_jump_precedence_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <algorithm>                                    // lower_bound, stable_sort, unique
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
                precedence_dispatch(p, moves, is_absolute_king_jump_precedence_t<Rules>{});

                //handle_precedence(moves, value_compare<Rules>{});
                auto const check_duplicate = is_remove_duplicates_v<Rules> && moves.size() > 1;
                if (check_duplicate) {
                        auto maxx = std::max_element(begin(moves), end(moves), [](auto const& L, auto const& R){
                                return L.num_pieces() < R.num_pieces();
                        });

                        if (maxx != end(moves) && maxx->num_pieces() >= large_jump_v<Rules>)
                                handle_uniqueness(moves);
                }
        }

private:
        template<class Position, class Sequence>
        using KingJump = Generate<Color, pieces::king, select::jump, Position, Sequence>;

        template<class Position, class Sequence>
        using PawnJump = Generate<Color, pieces::pawn, select::jump, Position, Sequence>;

        // no absolute king jump precedence
        template<class Position, class Sequence>
        auto precedence_dispatch(Position const& p, Sequence& moves, std::false_type) const
        {
                Propagate<select::jump, Position> back_tracker{p};
                KingJump<Position, Sequence>{back_tracker, moves}(p.kings(Color));
                PawnJump<Position, Sequence>{back_tracker, moves}(p.pawns(Color));
        }

        // absolute king jump precedence
        template<class Position, class Sequence>
        auto precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                Propagate<select::jump, Position> back_tracker{p};
                KingJump<Position, Sequence>{back_tracker, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJump<Position, Sequence>{back_tracker, moves}(p.pawns(Color));
        }

        template<class Sequence, class Compare>
        void handle_precedence(Sequence& moves, Compare cmp) const
        {
                if (moves.size() <= 1) return;
                std::stable_sort(begin(moves), end(moves), cmp);
                auto const keep = std::lower_bound(begin(moves), end(moves), moves.back(), cmp);
                moves.erase(begin(moves), keep);
        }

        template<class Sequence>
        void handle_uniqueness(Sequence& moves) const
        {
                if (moves.size() <= 1) return;
                std::stable_sort(begin(moves), end(moves));
                auto const drop = std::unique(begin(moves), end(moves));
                moves.erase(drop, end(moves));
        }

        template<class Rules>
        struct value_compare
        {
                template<class Move>
                auto operator()(Move const& L, Move const& R) const
                {
                        using Value = typename Rules::template value_type<Move>;
                        return Value{L} < Value{R};
                }
        };
};

}       // namespace successor
}       // namespace dctl
