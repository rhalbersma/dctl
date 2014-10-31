#pragma once
#include <dctl/rules/czech/precedence.hpp>      // less

namespace dctl {
namespace czech {

// http://www.damweb.cz/pravidla/cdfull.html

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_long_ranged_king = true;

        // optional
        using jump_precedence = precedence::less;
        static constexpr auto is_absolute_king_jump_precedence = true;
};

}       // namespace czech
}       // namespace dctl
