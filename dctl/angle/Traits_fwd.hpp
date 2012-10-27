#pragma once
#include <dctl/mpl/lazy.hpp>            // DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION

namespace dctl {
namespace angle {

// primary template definitions

DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_orthogonal)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_diagonal)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_up)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_down)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_left)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_right)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_positive)
DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(is_negative)

}       // namespace angle
}       // namespace dctl
