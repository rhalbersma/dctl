#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT

namespace dctl {

enum class Notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric   = 2
};

XSTD_PP_TTI_CONSTANT(notation, Notation::numeric)
XSTD_PP_TTI_CONSTANT(pushsep, '-')
XSTD_PP_TTI_CONSTANT(jumpsep, 'x')

}       // namespace dctl
