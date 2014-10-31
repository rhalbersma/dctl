#pragma once
#include <type_traits>  // integral_constant, remove_const

#define DCTL_PP_TTI_BOX_STATIC_CONSTANT(NAME)                                                   \
                                                                                                \
template<class T>                                                                               \
struct box_static_constant_ ## NAME                                                             \
{                                                                                               \
        static constexpr auto value = T::NAME;                                                  \
        using type = std::integral_constant<std::remove_const_t<decltype(value)>, value>;       \
};                                                                                              \
