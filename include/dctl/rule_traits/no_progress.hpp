#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT
#include <type_traits>          // integral_constant

namespace dctl {

XSTD_PP_TTI_CONSTANT(max_same_king_push, 0)

template<class Rules>
constexpr auto is_restricted_king_push_v = max_same_king_push_v<Rules> != 0;

template<class Rules>
using is_restricted_king_push_t = std::integral_constant<bool,
        is_restricted_king_push_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(max_reversible_moves, 0)

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

template<class Rules>
using is_restricted_reversible_moves_t = std::integral_constant<bool,
        is_restricted_reversible_moves_v<Rules>
>;

}       // namespace dctl
