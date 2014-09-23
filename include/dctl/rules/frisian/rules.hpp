#pragma once
#include <dctl/rules/frisian/precedence.hpp>    // less

namespace dctl {
namespace frisian {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_long_ranged_king = true;

        using jump_precedence = precedence::less;

        // optional
        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 6;
};

}       // namespace frisian
}       // namespace dctl
