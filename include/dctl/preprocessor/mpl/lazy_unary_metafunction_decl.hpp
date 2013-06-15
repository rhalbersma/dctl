#pragma once

#define DCTL_PP_LAZY_UNARY_METAFUNCTION_DECL(F)         \
namespace lazy {                                        \
template<class X>                                       \
struct F;                                               \
}       // namespace lazy
