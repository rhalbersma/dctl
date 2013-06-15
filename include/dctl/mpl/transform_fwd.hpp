#pragma once
#include <dctl/preprocessor/mpl/lazy_unary_metafunction_decl.hpp>   // DCTL_PP_LAZY_UNARY_METAFUNCTION_DECL
#include <dctl/preprocessor/mpl/lazy_binary_metafunction_decl.hpp>  // DCTL_PP_LAZY_BINARY_METAFUNCTION_DECL

namespace dctl {
namespace mpl {

// primary template declarations

DCTL_PP_LAZY_UNARY_METAFUNCTION_DECL(inverse)
DCTL_PP_LAZY_BINARY_METAFUNCTION_DECL(rotate)
DCTL_PP_LAZY_BINARY_METAFUNCTION_DECL(mirror)

}	// namespace mpl
}       // namespace dctl
