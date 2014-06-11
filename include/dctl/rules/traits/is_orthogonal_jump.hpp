#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace detail {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_orthogonal_jump, is_orthogonal_jump)

}       // namespace detail

template<class Rules>
using is_orthogonal_jump = detail::is_orthogonal_jump<Rules, std::false_type>;

template<class Rules>
using is_orthogonal_jump_t = typename is_orthogonal_jump<Rules>::type;

template<class Rules>
constexpr auto is_orthogonal_jump_v = is_orthogonal_jump_t<Rules>::value;

}       // namespace dctl
