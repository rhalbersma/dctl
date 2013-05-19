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
        typedef range::distance_N king_range;
        typedef directions::up pawn_jump_directions;
        typedef precedence::none jump_precedence;

        // additional rules
        typedef range::distance_1 land_range;
        typedef std::true_type is_jump_direction_reversal;
        typedef removal::en_passant jump_removal;
        typedef std::integral_constant<int, 4> initial_gap;
};

}       // namespace rules
}       // namespace dctl
