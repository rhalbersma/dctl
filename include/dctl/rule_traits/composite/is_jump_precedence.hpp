#pragma once
#include <dctl/rule_traits/optional/jump_precedence.hpp>        // jump_precedence_t

namespace dctl {

template<class Rules>
using is_jump_precedence_t = detail_jump_precedence::has_type_t<Rules>;

template<class Rules>
constexpr auto is_jump_precedence_v = detail_jump_precedence::has_type_v<Rules>;

}       // namespace dctl
