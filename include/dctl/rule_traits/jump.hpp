#pragma once
#include <dctl/rule_traits/promotion.hpp>
#include <dctl/rule_traits/range.hpp>
#include <xstd/pp/tti.hpp>                      // XSTD_PP_TTI_CONSTANT
#include <type_traits>                          // integral_constant, is_same

namespace dctl {

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)
XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)
XSTD_PP_TTI_CONSTANT(is_pawn_jump_king, true)
XSTD_PP_TTI_CONSTANT(is_en_passant_jump_removal, false)

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_en_passant_jump_removal_v<Rules> && std::is_same<king_range_category_t<Rules>, long_ranged_tag>::value
;

template<class Rules>
using is_reversible_king_jump_direction_t = std::integral_constant<bool,
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
        !(is_backward_pawn_jump_v<Rules> || std::is_same<promotion_category_t<Rules>, passing_promotion_tag>::value ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::integral_constant<bool,
        is_unambiguous_pawn_jump_v<Rules>>
;

}       // namespace dctl
