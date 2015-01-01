#pragma once
#include <dctl/rule_traits/optional/is_en_passant_jump_removal.hpp>     // is_en_passant_jump_removal_v
#include <dctl/rule_traits/required/king_range_category.hpp>            // is_long_ranged_king
#include <type_traits>                                                  // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_en_passant_jump_removal_v<Rules> && std::is_same<king_range_category_t<Rules>, long_ranged_tag>::value
;

template<class Rules>
using is_reversible_king_jump_direction_t = std::integral_constant<bool,
        is_reversible_king_jump_direction_v<Rules>>
;

}       // namespace dctl
