#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE
#include <type_traits>                  // false_type

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_en_passant_promotion, is_en_passant_promotion)

template<class Rules, class Default = std::false_type>
using is_en_passant_promotion_t = typename is_en_passant_promotion<Rules, Default>::type;

}       // namespace rules
}       // namespace dctl
