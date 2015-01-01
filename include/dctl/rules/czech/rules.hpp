#pragma once
#include <dctl/rules/czech/precedence.hpp>      // less
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace czech {

// http://www.damweb.cz/pravidla/cdfull.html

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        // optional
        using jump_precedence = precedence::less;
        static constexpr auto is_absolute_king_jump_precedence = true;
};

}       // namespace czech
}       // namespace dctl
