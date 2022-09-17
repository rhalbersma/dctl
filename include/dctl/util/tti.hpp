#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>  // conditional_t, void_t

#define DCTL_PP_TTI_CONSTANT(NAME, DEFAULT)                     \
                                                                \
template<class T, class = void>                                 \
inline constexpr static auto has_ ## NAME ## _v = false;        \
                                                                \
template<class T>                                               \
inline constexpr static auto has_ ## NAME ## _v<                \
        T, std::void_t<decltype(T::NAME)>                       \
> = true;                                                       \
                                                                \
struct default_ ## NAME                                         \
{                                                               \
        constexpr static auto NAME = DEFAULT;                   \
};                                                              \
                                                                \
template<class T>                                               \
inline constexpr auto NAME ## _v = std::conditional_t<          \
        has_ ## NAME ## _v<T>, T, default_ ## NAME              \
>::NAME;                                                        \

