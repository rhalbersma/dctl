#pragma once
#include <dctl/rule_traits/king_range.hpp>
#include <dctl/rule_traits/promotion.hpp>
#include <xstd/pp/tti.hpp>                      // XSTD_PP_TTI_CONSTANT
#include <experimental/type_traits>             // bool_constant (C++1z), is_same, false_type, true_type

namespace dctl {

XSTD_PP_TTI_CONSTANT(is_passing_capture, false)

template<class Rules>
using capture_category_t = is_passing_capture_t<Rules>;

using stopped_capture_tag = std::false_type;
using passing_capture_tag = std::true_type;

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)
XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)
XSTD_PP_TTI_CONSTANT(is_pawn_jump_king, true)

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_passing_capture_v<Rules> &&
        is_long_ranged_king_v<Rules>
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
