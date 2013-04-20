#pragma once
#include <dctl/mpl/transform_fwd.hpp>                           // inverse, rotate, mirror
#include <dctl/preprocessor/lazy_unary_metafunction_def.hpp>    // DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF
#include <dctl/preprocessor/lazy_binary_metafunction_def.hpp>   // DCTL_PP_LAZY_BINARY_METAFUNCTION_DEF

namespace dctl {
namespace mpl {

// primary template definitions

DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(inverse)
DCTL_PP_LAZY_BINARY_METAFUNCTION_DEF(rotate)
DCTL_PP_LAZY_BINARY_METAFUNCTION_DEF(mirror)

}	// namespace mpl
}       // namespace dctl
