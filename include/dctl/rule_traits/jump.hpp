#pragma once
#include <dctl/rule_traits/king_range.hpp>
#include <dctl/rule_traits/promotion.hpp>
#include <xstd/pp/tti.hpp>                      // XSTD_PP_TTI_CONSTANT
#include <type_traits>                          // bool_constant (C++1z), conditional, false_type, true_type

namespace dctl {

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)

struct  forward_pawn_jump_tag : std::false_type {};
struct backward_pawn_jump_tag : std::true_type {};

template<class Rules>
using pawn_jump_category_t = std::conditional_t<
        is_backward_pawn_jump_v<Rules>,
        backward_pawn_jump_tag,
         forward_pawn_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)

struct   diagonal_jump_tag : std::false_type {};
struct orthogonal_jump_tag : std::true_type {};

template<class Rules>
using jump_category_t = std::conditional_t<
        is_orthogonal_jump_v<Rules>,
        orthogonal_jump_tag,
          diagonal_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_pawn_jump_king, true)

XSTD_PP_TTI_CONSTANT(is_passing_capture, false)

struct stopped_capture_tag : std::false_type {};
struct passing_capture_tag : std::true_type {};

template<class Rules>
using capture_category_t = std::conditional_t<
        is_passing_capture_v<Rules>,
        passing_capture_tag,
        stopped_capture_tag
>;

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_passing_capture_v<Rules> && is_long_ranged_king_v<Rules>
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
        !(is_backward_pawn_jump_v<Rules> || is_passing_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::bool_constant<
        is_unambiguous_pawn_jump_v<Rules>
>;

}       // namespace dctl
