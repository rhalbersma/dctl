#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_backward_pawn_jump, is_backward_pawn_jump)

template<class Rules>
using is_backward_pawn_jump_t = typename is_backward_pawn_jump<Rules>::type;

}       // namespace rules
}       // namespace dctl
