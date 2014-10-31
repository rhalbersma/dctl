#pragma once
#include <dctl/rule_traits/optional/jump_precedence.hpp>        // jump_precedence_t

namespace dctl {

template<class Rules>
using is_jump_precedence_t = has_type_jump_precedence_t<Rules>;

template<class Rules>
constexpr auto is_jump_precedence_v = has_type_jump_precedence_v<Rules>;

}       // namespace dctl
