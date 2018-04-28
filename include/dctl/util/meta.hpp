#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/array>   // make_array
#include <type_traits>          // conditional_t, integral_constant, is_same_v

namespace dctl::core {
namespace meta {

template<class T, class... C>
struct mp_switch_impl;

template<class T, class... C>
using mp_switch = typename mp_switch_impl<T, C...>::type;

template<class K, class V>
struct mp_case;

template<class T, class K, class V, class... Cr>
struct mp_switch_impl<T, mp_case<K, V>, Cr...>
{
        using type = std::conditional_t<std::is_same_v<T, K>, V, mp_switch<T, Cr...>>;
};

template<class V>
struct mp_default;

template<class T, class V, class... Cr>
struct mp_switch_impl<T, mp_default<V>, Cr...>
{
        using type = V;
        static_assert(sizeof...(Cr) == 0, "mp_default must be final clause inside mp_switch");
};

struct mp_nonematch;

template<class T>
struct mp_switch_impl<T>
{
        using type = mp_nonematch;
};

template<class L>
struct foldl_logical_or;

template<template<class...> class L, class... T>
struct foldl_logical_or<L<T...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
        {
                return (... || fun(T{}));
        }
};

template<class L>
struct foldl_plus;

template<template<class...> class L, class... T>
struct foldl_plus<L<T...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
        {
                return (... + fun(T{}));
        }
};

template<class L>
struct foldl_comma;

template<template<class...> class L, class... T>
struct foldl_comma<L<T...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
        {
                (... , fun(T{}));
        }
};

template<class L>
struct foldl_bit_or;

template<template<class...> class L, class... T>
struct foldl_bit_or<L<T...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
        {
                return (... | fun(T{}));
        }
};

template<class L>
struct make_array;

template<template<class...> class L, class... T>
struct make_array<L<T...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
        {
                return std::experimental::make_array(fun(T{})...);
        }
};

}       // namespace meta
}       // namespace dctl::core
