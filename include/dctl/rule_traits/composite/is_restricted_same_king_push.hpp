#pragma once
#include <dctl/rule_traits/irregular/max_same_king_push.hpp>    // max_same_king_push
#include <type_traits>                                          // integral_constant

namespace dctl {

template<class Rules>
constexpr auto is_restricted_same_king_push_v = max_same_king_push_v<Rules> != 0;

template<class Rules>
using is_restricted_same_king_push_t = std::integral_constant<bool,
        is_restricted_same_king_push_v<Rules>
>;

}       // namespace dctl
