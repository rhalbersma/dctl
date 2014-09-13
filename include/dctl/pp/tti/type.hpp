#pragma once
#include <dctl/pp/tti/detail/box_type.hpp>      // DCTL_PP_TTI_BOX_TYPE
#include <dctl/pp/tti/detail/has_type.hpp>      // DCTL_PP_TTI_HAS_TYPE
#include <dctl/pp/tti/identity.hpp>             // identity
#include <type_traits>                          // conditional

#define DCTL_PP_TTI_TYPE(NAME, DEFAULT)                 \
                                                        \
DCTL_PP_TTI_HAS_TYPE(NAME)                              \
DCTL_PP_TTI_BOX_TYPE(NAME)                              \
                                                        \
template<class T>                                       \
using NAME ## _t = typename std::conditional_t<         \
        detail_ ## NAME ::has_type_v<T>,                \
        detail_ ## NAME ::box_type<T>,                  \
        dctl::tti::identity<DEFAULT>                    \
>::type;
