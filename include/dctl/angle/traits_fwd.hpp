#pragma once
#include <dctl/preprocessor/mpl/lazy_unary_metafunction_def.hpp>    // DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF
#include <dctl/preprocessor/mpl/lazy_binary_metafunction_def.hpp>   // DCTL_PP_LAZY_BINARY_METAFUNCTION_DEF

namespace dctl {
namespace angle {

// primary template definitions

DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_orthogonal)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_diagonal)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_up)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_down)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_left)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_right)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_positive)
DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(is_negative)

}       // namespace angle
}       // namespace dctl
