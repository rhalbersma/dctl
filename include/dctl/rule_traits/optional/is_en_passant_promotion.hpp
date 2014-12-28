#pragma once
#include <dctl/pp/tti/static_constant.hpp>      // DCTL_PP_TTI_STATIC_CONSTANT
#include <type_traits>                          // false_type, true_type

namespace dctl {

DCTL_PP_TTI_STATIC_CONSTANT(is_en_passant_promotion, false)

using apres_fini_type = std::false_type;
using en_passant_type = std::true_type;

}       // namespace dctl
