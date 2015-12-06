#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT
#include <type_traits>          // conditional, false_type, true_type

namespace dctl {

XSTD_PP_TTI_CONSTANT(is_long_ranged_king, false)

template<class Rules>
using king_range_category_t = is_long_ranged_king_t<Rules>;

using short_ranged_tag = std::false_type;
using long_ranged_tag = std::true_type;

XSTD_PP_TTI_CONSTANT(is_land_behind_piece, false)

template<class Rules>
using king_range_category_land_behind_piece_t = std::conditional_t<
        is_land_behind_piece_v<Rules>,
        short_ranged_tag,
        king_range_category_t<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_halt_behind_king, false)

template<class Rules>
using king_range_category_halt_behind_king_t = std::conditional_t<
        is_halt_behind_king_v<Rules>,
        short_ranged_tag,
        king_range_category_land_behind_piece_t<Rules>
>;

}       // namespace dctl
