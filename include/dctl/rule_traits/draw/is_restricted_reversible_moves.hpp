#pragma once
#include <dctl/rule_traits/draw/max_reversible_moves.hpp>       // max_reversible_moves
#include <type_traits>                                          // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

template<class Rules>
using is_restricted_reversible_moves_t = std::integral_constant<bool,
        is_restricted_reversible_moves_v<Rules>
>;

}       // namespace dctl
