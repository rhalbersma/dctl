#pragma once
#include <xstd/pp/tti/static_constant.hpp>      // XSTD_PP_TTI_STATIC_CONSTANT

namespace dctl {

enum class Notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric   = 2
};

XSTD_PP_TTI_STATIC_CONSTANT(notation, Notation::numeric)

}       // namespace dctl
