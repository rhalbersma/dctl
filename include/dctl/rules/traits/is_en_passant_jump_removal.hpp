#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace detail {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_en_passant_jump_removal, is_en_passant_jump_removal)

}       // namespace detail

template<class Rules>
using is_en_passant_jump_removal = detail::is_en_passant_jump_removal<Rules, std::false_type>;

template<class Rules>
using is_en_passant_jump_removal_t = typename is_en_passant_jump_removal<Rules>::type;

template<class Rules>
constexpr auto is_en_passant_jump_removal_v = is_en_passant_jump_removal_t<Rules>::value;

}       // namespace dctl
