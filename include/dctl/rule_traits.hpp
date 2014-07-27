#pragma once
#include <dctl/rules/max_reversible_moves.hpp>
#include <dctl/rules/max_same_king_push.hpp>

#include <dctl/rules/is_restricted_reversible_moves.hpp>
#include <dctl/rules/is_restricted_same_king_push.hpp>

#include <dctl/rules/draw.hpp>

#include <dctl/pp/tti/static_constant.hpp>      // DCTL_PP_TTI_STATIC_CONSTANT, DCTL_PP_TTI_HAS_NO_DEFAULT
#include <type_traits>                          // integral_constant

namespace dctl {

DCTL_PP_TTI_STATIC_CONSTANT(is_backward_pawn_jump, DCTL_PP_TTI_HAS_NO_DEFAULT)
DCTL_PP_TTI_STATIC_CONSTANT(is_jump_precedence,    DCTL_PP_TTI_HAS_NO_DEFAULT)
DCTL_PP_TTI_STATIC_CONSTANT(is_long_ranged_king,   DCTL_PP_TTI_HAS_NO_DEFAULT)

DCTL_PP_TTI_STATIC_CONSTANT(initial_gap,                       2    )
DCTL_PP_TTI_STATIC_CONSTANT(is_absolute_king_jump_precedence,  false)
DCTL_PP_TTI_STATIC_CONSTANT(is_directly_halt_after_final_king, false)
DCTL_PP_TTI_STATIC_CONSTANT(is_directly_land_after_piece,      false)
DCTL_PP_TTI_STATIC_CONSTANT(is_en_passant_jump_removal,        false)
DCTL_PP_TTI_STATIC_CONSTANT(is_en_passant_promotion,           false)
DCTL_PP_TTI_STATIC_CONSTANT(is_orthogonal_jump,                false)
DCTL_PP_TTI_STATIC_CONSTANT(is_pawn_jump_king,                 true )

DCTL_PP_TTI_STATIC_CONSTANT(is_remove_duplicates,              true )

template<class Rules>
constexpr auto is_long_ranged_land_after_piece_v =
        is_long_ranged_king_v<Rules> && !is_directly_land_after_piece_v<Rules>
;

template<class Rules>
using is_long_ranged_land_after_piece_t = std::integral_constant<bool,
        is_long_ranged_land_after_piece_v<Rules>>
;

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_en_passant_jump_removal_v<Rules> && is_long_ranged_king_v<Rules>
;

template<class Rules>
using is_reversible_king_jump_direction_t = std::integral_constant<bool,
        is_reversible_king_jump_direction_v<Rules>>
;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_en_passant_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::integral_constant<bool,
        is_unambiguous_pawn_jump_v<Rules>>
;

template<class Rules>
constexpr auto large_jump_v =
        ((is_orthogonal_jump_v<Rules> && is_backward_pawn_jump_v<Rules>) ||
         (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>) ||
         (is_reversible_king_jump_direction_v<Rules> && is_backward_pawn_jump_v<Rules>)) ? 3 : 4
;

template<class Rules>
using large_jump_t = std::integral_constant<int, large_jump_v<Rules>>;

}       // namespace dctl
