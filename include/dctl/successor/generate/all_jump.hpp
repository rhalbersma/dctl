#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // Generate (king jump specialization)
#include <dctl/successor/generate/pawn_jump.hpp>        // Generate (pawn jump specialization)
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <dctl/rule_traits.hpp>                         // is_absolute_king_jump_precedence_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <algorithm>                                    // upper_bound, stable_sort, unique
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

                auto const check_precedence = is_jump_precedence_v<Rules>;
                if (check_precedence)
                        handle_precedence(moves, precedence_greater{});

                auto const check_duplicate = is_remove_duplicates_v<Rules>;// && moves.size() > 1;
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
                Tracker<Color, Position> tracker{p};
                KingJump<Position, Sequence>{tracker, moves}(p.kings(Color));
                PawnJump<Position, Sequence>{tracker, moves}(p.pawns(Color));
        }

        // absolute king jump precedence
        template<class Position, class Sequence>
        auto precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                Tracker<Color, Position> tracker{p};
                KingJump<Position, Sequence>{tracker, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJump<Position, Sequence>{tracker, moves}(p.pawns(Color));
        }

        template<class Sequence, class Compare>
        void handle_precedence(Sequence& moves, Compare cmp) const
        {
                std::stable_sort(begin(moves), end(moves), cmp);
                auto const drop = std::upper_bound(begin(moves), end(moves), moves.front(), cmp);
                moves.erase(drop, end(moves));
        }

        template<class Sequence>
        void handle_uniqueness(Sequence& moves) const
        {
                std::stable_sort(begin(moves), end(moves));
                auto const drop = std::unique(begin(moves), end(moves));
                moves.erase(drop, end(moves));
        }

        struct precedence_greater
        {
                template<class Move>
                auto operator()(Move const& lhs, Move const& rhs) const
                {
                        using less = jump_precedence_t<rules_type_t<Move>>;
                        return less{}(rhs, lhs);
                }
        };
};

}       // namespace successor
}       // namespace dctl
