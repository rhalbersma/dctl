#pragma once
#include <dctl/pp/tti/static_constant_no_default.hpp>   // DCTL_PP_TTI_STATIC_CONSTANT_NO_DEFAULT
#include <type_traits>                                  // false_type, true_type

namespace dctl {

DCTL_PP_TTI_STATIC_CONSTANT_NO_DEFAULT(is_long_ranged_king)

using short_ranged_type = std::false_type;
using  long_ranged_type = std::true_type;

}       // namespace dctl
