#pragma once

#define DCTL_PP_LAZY_BINARY_METAFUNCTION_DECL(F)        \
namespace lazy {                                        \
template<class X1, class X2>                            \
struct F;                                               \
}       // namespace lazy
