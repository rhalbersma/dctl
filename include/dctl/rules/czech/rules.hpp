#pragma once
#include <dctl/rules/czech/value.hpp>   // Value

namespace dctl {
namespace czech {

// http://www.damweb.cz/pravidla/cdfull.html

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_jump_precedence = true;
        static constexpr auto is_long_ranged_king = true;

        using value_type = Value;

        // optional
        static constexpr auto is_absolute_king_jump_precedence = true;
};

}       // namespace czech
}       // namespace dctl
