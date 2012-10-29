#pragma once
#include <dctl/mpl/lazy.hpp>            // DCTL_PP_DECLARE_UNARY_METAFUNCTION, DCTL_PP_DECLARE_BINARY_METAFUNCTION

namespace dctl {
namespace mpl {

// primary template declarations

DCTL_PP_DECLARE_LAZY_UNARY_METAFUNCTION(inverse)
DCTL_PP_DECLARE_LAZY_BINARY_METAFUNCTION(rotate)
DCTL_PP_DECLARE_LAZY_BINARY_METAFUNCTION(mirror)

}	// namespace mpl
}       // namespace dctl
