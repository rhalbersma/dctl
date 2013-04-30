#pragma once

#define DCTL_PP_LAZY_BINARY_METAFUNCTION_DECL(F)        \
namespace lazy {                                        \
template<typename X1, typename X2>                      \
struct F;                                               \
}       // namespace lazy
