#pragma once
#include <dctl/rule_traits/regular/is_backward_pawn_jump.hpp>                   // is_backward_pawn_jump_v
#include <dctl/rule_traits/irregular/is_en_passant_promotion.hpp>               // is_en_passant_promotion_v
#include <dctl/rule_traits/irregular/is_orthogonal_jump.hpp>                    // is_orthogonal_jump_v
#include <dctl/rule_traits/composite/is_reversible_king_jump_direction.hpp>     // is_reversible_king_jump_direction_v
#include <type_traits>                                                          // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_en_passant_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::integral_constant<bool,
        is_unambiguous_pawn_jump_v<Rules>>
;

}       // namespace dctl
