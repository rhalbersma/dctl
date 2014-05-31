#pragma once
#include <boost/tti/member_type.hpp>    // BOOST_TTI_TRAIT_MEMBER_TYPE

namespace dctl {
namespace rules {

BOOST_TTI_TRAIT_MEMBER_TYPE(is_long_ranged_king, is_long_ranged_king)

template<class Rules>
using is_long_ranged_king_t = typename is_long_ranged_king<Rules>::type;

}       // namespace rules
}       // namespace dctl
