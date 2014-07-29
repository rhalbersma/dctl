#pragma once
#include <dctl/rule_traits/is_en_passant_jump_removal.hpp>      // is_en_passant_jump_removal_v
#include <dctl/rule_traits/is_long_ranged_king.hpp>             // is_long_ranged_king
#include <type_traits>                                          // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_en_passant_jump_removal_v<Rules> && is_long_ranged_king_v<Rules>
;

template<class Rules>
using is_reversible_king_jump_direction_t = std::integral_constant<bool,
        is_reversible_king_jump_direction_v<Rules>>
;

}       // namespace dctl
