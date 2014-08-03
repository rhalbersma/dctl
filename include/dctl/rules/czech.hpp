#pragma once
#include <dctl/rules/czech_fwd.hpp>             // Czech
#include <dctl/successor/value/czech.hpp>       // Value (Czech specialization)

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_jump_precedence = true;
        static constexpr auto is_long_ranged_king = true;

        // optional
        static constexpr auto is_absolute_king_jump_precedence = true;
};

}       // namespace rules
}       // namespace dctl
