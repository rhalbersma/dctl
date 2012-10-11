#pragma once
#include "lazy.hpp"              		// LAZY_UNARY_METAFUNCTION, LAZY_BINARY_METAFUNCTION

namespace dctl {
namespace mpl {

LAZY_UNARY_METAFUNCTION(inverse)
LAZY_BINARY_METAFUNCTION(rotate)
LAZY_BINARY_METAFUNCTION(mirror)

}		// namespace mpl
}       // namespace dctl
