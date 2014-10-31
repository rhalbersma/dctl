#pragma once
#include <type_traits>  // is_same

#define DCTL_PP_TTI_MISSING_STATIC_CONSTANT(NAME)                       \
                                                                        \
template<class T = decltype(nullptr)>                                   \
struct missing_static_constant_ ## NAME                                 \
{                                                                       \
        static_assert(                                                  \
                !std::is_same<T, decltype(nullptr)>::value,             \
                "DCTL error: missing static constant '" #NAME "'."      \
        );                                                              \
};                                                                      \
