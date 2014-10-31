#pragma once
#include <dctl/pp/tti/detail/box_static_constant.hpp>           // DCTL_PP_TTI_BOX_STATIC_CONSTANT
#include <dctl/pp/tti/detail/has_static_constant.hpp>           // DCTL_PP_TTI_HAS_STATIC_CONSTANT
#include <dctl/pp/tti/detail/missing_static_constant.hpp>       // DCTL_PP_TTI_MISSING_STATIC_CONSTANT
#include <type_traits>                                          // conditional

#define DCTL_PP_TTI_STATIC_CONSTANT_NO_DEFAULT(NAME)    \
                                                        \
DCTL_PP_TTI_HAS_STATIC_CONSTANT(NAME)                   \
DCTL_PP_TTI_BOX_STATIC_CONSTANT(NAME)                   \
DCTL_PP_TTI_MISSING_STATIC_CONSTANT(NAME)               \
                                                        \
template<class T>                                       \
using NAME ## _t = typename std::conditional_t<         \
        has_static_constant_ ## NAME ## _v<T>,          \
        box_static_constant_ ## NAME <T>,               \
        missing_static_constant_ ## NAME <>             \
>::type;                                                \
                                                        \
template<class T>                                       \
constexpr auto NAME ## _v = NAME ## _t<T>::value;       \
