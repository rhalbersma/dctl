#pragma once
#include <dctl/rule_traits/composite/is_reversible_king_jump_direction.hpp>     // is_reversible_king_jump_direction
#include <dctl/rule_traits/optional/is_orthogonal_jump.hpp>                     // is_orthogonal_jump
#include <dctl/rule_traits/optional/promotion_category.hpp>                     // promotion_category, passing_promotion_tag
#include <dctl/rule_traits/required/is_backward_pawn_jump.hpp>                  // is_backward_pawn_jump
#include <type_traits>                                                          // integral_constant, is_same

namespace dctl {

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || std::is_same<promotion_category<Rules>, passing_promotion_tag>::value ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump_t = std::integral_constant<bool,
        is_unambiguous_pawn_jump_v<Rules>>
;

}       // namespace dctl
