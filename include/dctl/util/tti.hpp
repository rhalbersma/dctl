#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>  // conditional_t, void_t
#include <utility>      // declval

#define DCTL_PP_TTI_CONSTANT(NAME, DEFAULT)                     \
                                                                \
template<class T, class = void>                                 \
constexpr static auto has_ ## NAME ## _v = false;               \
                                                                \
template<class T>                                               \
constexpr static auto has_ ## NAME ## _v<                       \
        T, std::void_t<decltype(std::declval<T>().NAME)>        \
> = true;                                                       \
                                                                \
struct default_ ## NAME                                         \
{                                                               \
        constexpr static auto NAME = DEFAULT;                   \
};                                                              \
                                                                \
template<class T>                                               \
constexpr auto NAME ## _v = std::conditional_t<                 \
        has_ ## NAME ## _v<T>, T, default_ ## NAME              \
>::NAME;                                                        \

