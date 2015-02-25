#pragma once
#include <dctl/rule_traits/precedence/relational.hpp>   // less_t

namespace dctl {
namespace precedence {

template<class Rules>
using is_less_t = has_type_less_t<Rules>;

template<class Rules>
constexpr auto is_less_v = has_type_less_v<Rules>;

}       // namespace precedence
}       // namespace dctl
