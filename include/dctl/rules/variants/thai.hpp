#pragma once
#include <dctl/rules/variants/thai_fwd.hpp>     // Thai
#include <dctl/rules/types/range.hpp>           // distance_1
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (Thai specialization)
#include <type_traits>                          // true_type, integral_constant

namespace dctl {
namespace rules {

struct Thai
{
        // main rules
        using is_long_ranged_king = std::true_type;
        using is_backward_pawn_jump = std::false_type;
        using precedence_jump = precedence::none;

        // additional rules
        using range_land = range::distance_1;
        using is_en_passant_jump_removal = std::true_type;
        using initial_gap = std::integral_constant<int, 4>;
};

}       // namespace rules
}       // namespace dctl
