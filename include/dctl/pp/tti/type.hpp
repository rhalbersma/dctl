#pragma once
#include <dctl/pp/tti/detail/box_type.hpp>      // DCTL_PP_TTI_BOX_TYPE
#include <dctl/pp/tti/detail/has_type.hpp>      // DCTL_PP_TTI_HAS_TYPE
#include <xstd/type_traits.hpp>                 // type_is
#include <type_traits>                          // conditional

#define DCTL_PP_TTI_TYPE(NAME, DEFAULT)                 \
                                                        \
DCTL_PP_TTI_HAS_TYPE(NAME)                              \
DCTL_PP_TTI_BOX_TYPE(NAME)                              \
                                                        \
template<class T>                                       \
using NAME ## _t = typename std::conditional_t<         \
        has_type_ ## NAME ## _v<T>,                     \
        box_type_ ## NAME <T>,                          \
        xstd::type_is<DEFAULT>                          \
>::type;                                                \
                                                        \
template<class T>                                       \
constexpr auto NAME ## _v = NAME ## _t<T>::value;       \
