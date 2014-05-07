#pragma once
#include <dctl/rules/variants/thai_fwd.hpp>     // Thai
#include <dctl/rules/types/range.hpp>           // distance_1, distance_N
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/rules/types/phase.hpp>           // en_passant
#include <dctl/successor/value.hpp>             // Value (Thai specialization)
#include <type_traits>                          // true_type, integral_constant

namespace dctl {
namespace rules {

struct Thai
{
        // main rules
        typedef range::distance_N range_king;
        typedef directions::up directions_pawn_jump;
        typedef precedence::none precedence_jump;

        // additional rules
        typedef range::distance_1 range_land;
        typedef std::true_type directions_is_reversal;
        using jump_removal_phase = en_passant;
        using initial_gap = std::integral_constant<int, 4>;
};

}       // namespace rules
}       // namespace dctl
