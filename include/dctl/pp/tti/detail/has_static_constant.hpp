#pragma once
#include <xstd/type_traits.hpp> // void_t
#include <type_traits>          // false_type, true_type

#define DCTL_PP_TTI_HAS_STATIC_CONSTANT(NAME)                                                           \
                                                                                                        \
template<class, class = void>                                                                           \
struct has_static_constant_ ## NAME: std::false_type {};                                                \
                                                                                                        \
template<class T>                                                                                       \
struct has_static_constant_ ## NAME<T, xstd::void_t<decltype(T::NAME)>>: std::true_type {};             \
                                                                                                        \
template<class T>                                                                                       \
using has_static_constant_ ## NAME ## _t = typename has_static_constant_ ## NAME <T>::type;             \
                                                                                                        \
template<class T>                                                                                       \
constexpr auto has_static_constant_ ## NAME ## _v = has_static_constant_ ## NAME ## _t<T>::value;       \
