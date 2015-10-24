#pragma once
#include <dctl/rule_traits/king_range.hpp>
#include <dctl/rule_traits/promotion.hpp>
#include <xstd/pp/tti.hpp>                      // XSTD_PP_TTI_CONSTANT
#include <experimental/type_traits>             // bool_constant (C++1z), is_same

namespace dctl {

struct stopped_capture_tag {};
struct passing_capture_tag {};

XSTD_PP_TTI_TYPENAME(capture_category, stopped_capture_tag)

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)
XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)
XSTD_PP_TTI_CONSTANT(is_pawn_jump_king, true)

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        std::experimental::is_same_v<capture_category_t<Rules>, passing_capture_tag> &&
        std::experimental::is_same_v<king_range_category_t<Rules>, long_ranged_tag>
;

template<class Rules>
using is_reversible_king_jump_direction_t = std::bool_constant<
        is_reversible_king_jump_direction_v<Rules>>
;

template<class Rules>
constexpr auto large_jump_v =
        ((is_orthogonal_jump_v<Rules> && is_backward_pawn_jump_v<Rules>) ||
         (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>) ||
         (is_reversible_king_jump_direction_v<Rules> && is_backward_pawn_jump_v<Rules>)) ? 3 : 4
;

template<class Rules>
using large_jump_t = std::integral_constant<int, large_jump_v<Rules>>;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || std::experimental::is_same_v<promotion_category_t<Rules>, passing_promotion_tag> ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::bool_constant<
        is_unambiguous_pawn_jump_v<Rules>
>;

}       // namespace dctl
