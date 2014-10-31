#pragma once
#include <xstd/type_traits.hpp> // void_t
#include <type_traits>          // false_type, true_type

#define DCTL_PP_TTI_HAS_TYPE(NAME)                                              \
                                                                                \
template<class, class = void>                                                   \
struct has_type_ ## NAME: std::false_type {};                                   \
                                                                                \
template<class T>                                                               \
struct has_type_ ## NAME<T, xstd::void_t<typename T::NAME>>: std::true_type {}; \
                                                                                \
template<class T>                                                               \
using has_type_ ## NAME ## _t = typename has_type_ ## NAME <T>::type;           \
                                                                                \
template<class T>                                                               \
constexpr auto has_type_ ## NAME ## _v = has_type_ ## NAME ## _t<T>::value;     \
