#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE

namespace dctl {
namespace detail {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_long_ranged_king, is_long_ranged_king)

}       // namespace detail

template<class Rules>
using is_long_ranged_king = detail::is_long_ranged_king<Rules>;

template<class Rules>
using is_long_ranged_king_t = typename is_long_ranged_king<Rules>::type;

template<class Rules>
constexpr auto is_long_ranged_king_v = is_long_ranged_king_t<Rules>::value;

}       // namespace dctl
