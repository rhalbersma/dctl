#pragma once
#include <dctl/rules/irregular/is_en_passant_jump_removal.hpp>  // is_en_passant_jump_removal
#include <dctl/rules/regular/is_long_ranged_king.hpp>           // is_long_ranged_king
#include <type_traits>                                          // integral_constant

namespace dctl {

template<class Rules>
struct is_reversible_king_jump_direction
:
        std::integral_constant<bool,
                is_en_passant_jump_removal_v<Rules> &&
                is_long_ranged_king_v<Rules>
        >
{};

template<class Rules>
using is_reversible_king_jump_direction_t = typename is_reversible_king_jump_direction<Rules>::type;

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v = is_reversible_king_jump_direction_t<Rules>::value;

}       // namespace dctl
