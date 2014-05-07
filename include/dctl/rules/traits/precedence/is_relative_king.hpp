#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_relative_king_jump_precedence, is_relative_king_jump_precedence)

template<class Rules, class Default = std::false_type>
using is_relative_king_jump_precedence_t = typename is_relative_king_jump_precedence<Rules, Default>::type;

}       // namespace rules
}       // namespace dctl
