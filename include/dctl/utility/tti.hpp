#pragma once
#include <type_traits>  // conditional, void_t

#define DCTL_PP_TTI_CONSTANT(NAME, DEFAULT)             \
                                                        \
template<class T, class = void>                         \
static constexpr auto has_ ## NAME ## _v = false;       \
                                                        \
template<class T>                                       \
static constexpr auto has_ ## NAME ## _v<               \
        T, std::void_t<decltype(T::NAME)>               \
> = true;                                               \
                                                        \
struct default_ ## NAME                                 \
{                                                       \
        static constexpr auto NAME = DEFAULT;           \
};                                                      \
                                                        \
template<class T>                                       \
constexpr auto NAME ## _v = std::conditional_t<         \
        has_ ## NAME ## _v<T>, T, default_ ## NAME      \
>::NAME;                                                \
