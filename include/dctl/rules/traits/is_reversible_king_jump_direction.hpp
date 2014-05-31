#pragma once
#include <dctl/rules/traits/is_long_ranged_king.hpp>            // is_long_ranged_king
#include <dctl/rules/traits/is_en_passant_jump_removal.hpp>     // is_en_passant_jump_removal
#include <type_traits>                                          // integral_constant

namespace dctl {
namespace rules {

template<class Rules>
struct is_reversible_king_jump_direction
:
        std::integral_constant<bool,
                is_long_ranged_king_t<Rules>::value &&
                is_en_passant_jump_removal_t<Rules>::value
        >
{};

template<class Rules>
using is_reversible_king_jump_direction_t = typename is_reversible_king_jump_direction<Rules>::type;

}       // namespace rules
}       // namespace dctl
