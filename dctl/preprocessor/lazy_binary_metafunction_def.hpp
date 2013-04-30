#pragma once

#define DCTL_PP_LAZY_BINARY_METAFUNCTION_DEF(F)         \
namespace lazy {                                        \
template<typename X1, typename X2>                      \
struct F                                                \
:                                                       \
        F<typename X1::type, typename X2::type>         \
{};                                                     \
}       // namespace lazy
