#pragma once

#define DCTL_PP_LAZY_UNARY_METAFUNCTION_DEF(F)          \
namespace lazy {                                        \
template<typename X>                                    \
struct F                                                \
:                                                       \
        F<typename X::type>                             \
{};                                                     \
}       // namespace lazy
