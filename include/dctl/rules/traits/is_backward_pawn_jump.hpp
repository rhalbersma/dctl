#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE

namespace dctl {
namespace detail {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_backward_pawn_jump, is_backward_pawn_jump)

}       // namespace detail

template<class Rules>
using is_backward_pawn_jump = detail::is_backward_pawn_jump<Rules>;

template<class Rules>
using is_backward_pawn_jump_t = typename is_backward_pawn_jump<Rules>::type;

template<class Rules>
constexpr auto is_backward_pawn_jump_v = is_backward_pawn_jump_t<Rules>::value;

}       // namespace dctl
