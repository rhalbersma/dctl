#pragma once
#include <dctl/rules/irregular/is_orthogonal_jump.hpp>     // jump_removal
#include <dctl/rules/composite/is_reversible_king_jump_direction.hpp> // is_reversible_king_jump_direction
#include <dctl/rules/regular/is_backward_pawn_jump.hpp> // pawn_jump
#include <type_traits>                                  // integral_constant

namespace dctl {

template<class Rules>
struct large_jump
:
        std::conditional<
                is_reversible_king_jump_direction_v<Rules> ||
                (is_backward_pawn_jump_v<Rules> && is_orthogonal_jump_v<Rules>),
                std::integral_constant<int, 3>,
                std::integral_constant<int, 4>
        >::type
{};

}       // namespace dctl
