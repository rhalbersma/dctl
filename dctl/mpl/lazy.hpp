#pragma once

#define DCTL_PP_DECLARE_UNARY_METAFUNCTION(F)           \
template<typename X>                                    \
struct F;

#define DCTL_PP_DECLARE_BINARY_METAFUNCTION(F)          \
template<typename X1, typename X2>                      \
struct F;

#define DCTL_PP_DEFINE_LAZY_UNARY_METAFUNCTION(F)       \
template<typename X>                                    \
struct F                                                \
:                                                       \
        F<typename X::type>                             \
{};

#define DCTL_PP_DEFINE_LAZY_BINARY_METAFUNCTION(F)      \
template<typename X1, typename X2>                      \
struct F                                                \
:                                                       \
        F<typename X1::type, typename X2::type>         \
{};
