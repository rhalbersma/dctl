#pragma once
#include <dctl/rules/frisian/value.hpp> // Value

namespace dctl {
namespace frisian {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_jump_precedence = true;
        static constexpr auto is_long_ranged_king = true;

        template<class Move>
        using value_type = Value<Move>;

        // optional
        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 6;
};

}       // namespace frisian
}       // namespace dctl
