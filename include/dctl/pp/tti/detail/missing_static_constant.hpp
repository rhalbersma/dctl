#pragma once
#include <type_traits>                  // is_integral

#define DCTL_PP_TTI_MISSING_STATIC_CONSTANT(NAME)                       \
                                                                        \
namespace detail_ ## NAME  {                                            \
                                                                        \
template<class T = decltype(nullptr)>                                   \
struct missing_static_constant                                          \
{                                                                       \
        static_assert(                                                  \
                std::is_integral<T>::value,                             \
                "DCTL error: missing static constant '" #NAME "'."      \
        );                                                              \
};                                                                      \
                                                                        \
}       /* namespace detail_ ## NAME */
