#pragma once
#include <dctl/rule_traits/precedence/relational.hpp>   // has_type_less_v
#include <type_traits>                                  // integral_constant

namespace dctl {
namespace precedence {

template<class Rules>
constexpr auto is_trivial_v = !has_type_less_v<Rules>;

template<class Rules>
using is_trivial_t = std::integral_constant<bool, is_trivial_v<Rules>>;

}       // namespace precedence
}       // namespace dctl
