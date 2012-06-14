#pragma once
#include "../mpl/lazy.hpp"              // LAZY_UNARY_METAFUNCTION, LAZY_BINARY_METAFUNCTION

namespace dctl {

LAZY_UNARY_METAFUNCTION(inverse)
LAZY_BINARY_METAFUNCTION(rotate)
LAZY_BINARY_METAFUNCTION(mirror)

}       // namespace dctl
