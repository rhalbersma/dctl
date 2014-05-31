#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_en_passant_jump_removal, is_en_passant_jump_removal)

template<class Rules, class Default = std::false_type>
using is_en_passant_jump_removal_t = typename is_en_passant_jump_removal<Rules, Default>::type;

}       // namespace rules
}       // namespace dctl
