#pragma once
#include <dctl/mpl/lazy.hpp>            // DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION, DCTL_PP_DEFINE_LAZY_BINARY_METAFUNCTION
#include <dctl/mpl/transform_fwd.hpp>   // inverse, rotate, mirror

namespace dctl {
namespace mpl {

// primary template definitions

DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(inverse)
DCTL_PP_DEFINE_LAZY_BINARY_METAFUNCTION(rotate)
DCTL_PP_DEFINE_LAZY_BINARY_METAFUNCTION(mirror)

}	// namespace mpl
}       // namespace dctl
