#pragma once
#include <dctl/rule_traits/optional/is_directly_land_after_piece.hpp>   // is_directly_land_after_piece_v
#include <dctl/rule_traits/required/king_range_category.hpp>            // king_range_category
#include <type_traits>                                                  // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_long_ranged_land_after_piece_v =
        std::is_same<king_range_category_t<Rules>, long_ranged_tag>::value && !is_directly_land_after_piece_v<Rules>
;

template<class Rules>
using is_long_ranged_land_after_piece_t = std::integral_constant<bool,
        is_long_ranged_land_after_piece_v<Rules>>
;

}       // namespace dctl
