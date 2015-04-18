#pragma once
#include <dctl/rule_traits/composite/king_range_category_land_behind_piece.hpp> // is_land_behind_piece
#include <dctl/rule_traits/optional/is_halt_behind_king.hpp>                    // king_range_category, short_ranged_tag
#include <type_traits>                                                          // conditional

namespace dctl {

template<class Rules>
using king_range_category_halt_behind_king_t = std::conditional_t<
        is_halt_behind_king_v<Rules>,
        short_ranged_tag,
        king_range_category_land_behind_piece_t<Rules>
>;

}       // namespace dctl
