#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace detail {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_en_passant_promotion, is_en_passant_promotion)

}       // namespace detail

template<class Rules>
using is_en_passant_promotion = detail::is_en_passant_promotion<Rules, std::false_type>;

template<class Rules>
using is_en_passant_promotion_t = typename is_en_passant_promotion<Rules>::type;

template<class Rules>
constexpr auto is_en_passant_promotion_v = is_en_passant_promotion_t<Rules>::value;

}       // namespace dctl
