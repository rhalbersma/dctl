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
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;
        using precedence_jump = precedence::none;

        // additional rules
        using range_land = range::distance_1;
        static constexpr auto is_en_passant_jump_removal = true;
        using initial_gap = std::integral_constant<int, 4>;
};

}       // namespace rules
}       // namespace dctl
