#pragma once
#include <type_traits>
#include <dctl/rules/variants/thai_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value.hpp>
#include <dctl/notation/thai.hpp>

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
        typedef std::true_type is_jump_direction_reversal;
        typedef phase::en_passant phase_capture;
        typedef std::integral_constant<int, 4> initial_gap;
};

}       // namespace rules
}       // namespace dctl
