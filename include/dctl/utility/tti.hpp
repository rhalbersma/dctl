#pragma once
#include <experimental/type_traits>     // detected_or_t
#include <type_traits>                  // integral_constant, decay

#define DCTL_PP_TTI_CONSTANT(NAME, DEFAULT)                                                                     \
                                                                                                                \
template<class T>                                                                                               \
using NAME ## _constant = std::integral_constant<std::decay_t<decltype(T::NAME)>, T::NAME>;                     \
                                                                                                                \
using NAME ## _default = std::integral_constant<decltype(DEFAULT), DEFAULT>;                                    \
                                                                                                                \
template<class T>                                                                                               \
constexpr auto NAME ## _v = std::experimental::detected_or_t<NAME ## _default, NAME ## _constant, T>::value;    \


#define DCTL_PP_TTI_TYPENAME(NAME, DEFAULT)                                                     \
                                                                                                \
template<class T>                                                                               \
using NAME ## _typename = typename T::NAME ## _type;                                            \
                                                                                                \
using NAME ## _default = DEFAULT;                                                               \
                                                                                                \
template<class T>                                                                               \
using NAME ## _t = std::experimental::detected_or_t<NAME ## _default, NAME ## _typename, T>;    \

