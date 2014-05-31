#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_orthogonal_jump, is_orthogonal_jump)

template<class Rules, class Default = std::false_type>
using is_orthogonal_jump_t = typename is_orthogonal_jump<Rules, Default>::type;

}       // namespace rules
}       // namespace dctl
