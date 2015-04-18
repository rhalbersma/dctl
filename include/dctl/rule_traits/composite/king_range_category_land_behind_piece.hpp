#pragma once
#include <dctl/rule_traits/optional/is_land_behind_piece.hpp>   // is_land_behind_piece
#include <dctl/rule_traits/required/king_range_category.hpp>    // king_range_category, short_ranged_tag
#include <type_traits>                                          // conditional

namespace dctl {

template<class Rules>
using king_range_category_land_behind_piece_t = std::conditional_t<
        is_land_behind_piece_v<Rules>,
        short_ranged_tag,
        king_range_category_t<Rules>
>;

}       // namespace dctl
