#pragma once
#include <dctl/rule_traits/optional/is_directly_land_after_piece.hpp>   // is_directly_land_after_piece_v
#include <dctl/rule_traits/required/is_long_ranged_king.hpp>            // is_long_ranged_king_v
#include <type_traits>                                                  // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_long_ranged_land_after_piece_v =
        is_long_ranged_king_v<Rules> && !is_directly_land_after_piece_v<Rules>
;

template<class Rules>
using is_long_ranged_land_after_piece_t = std::integral_constant<bool,
        is_long_ranged_land_after_piece_v<Rules>>
;

}       // namespace dctl
