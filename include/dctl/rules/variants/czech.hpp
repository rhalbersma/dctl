#pragma once
#include <dctl/rules/variants/czech_fwd.hpp>    // Czech
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)
#include <type_traits>                          // true_type

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        // main rules
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;
        using precedence_jump = precedence::none;

        // additional rules
        using is_absolute_king_jump_precedence = std::true_type;
};

}       // namespace rules
}       // namespace dctl
