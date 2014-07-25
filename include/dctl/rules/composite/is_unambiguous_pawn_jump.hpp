#pragma once
#include <dctl/rules/regular/is_backward_pawn_jump.hpp>         // is_backward_pawn_jump_v
#include <dctl/rules/irregular/is_en_passant_promotion.hpp>     // is_en_passant_promotion_v
#include <type_traits>                                          // integral_constant

namespace dctl {

template<class Rules>
struct is_unambiguous_pawn_jump
:
        std::integral_constant<bool,
                !is_backward_pawn_jump_v<Rules> && !is_en_passant_promotion_v<Rules>
        >
{};

template<class Rules>
using is_unambiguous_pawn_jump_t = typename is_unambiguous_pawn_jump<Rules>::type;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v = is_unambiguous_pawn_jump_t<Rules>::value;

}       // namespace dctl
