#pragma once
#include <dctl/rules/frisian/precedence.hpp>    // less
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace frisian {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;
        using king_range_category = long_ranged_tag;

        // optional
        using jump_precedence = precedence::less;
        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 6;
};

}       // namespace frisian
}       // namespace dctl
