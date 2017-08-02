#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/array>   // make_array
#include <tuple>                // tuple
#include <type_traits>          // conditional_t, integral_constant, is_same_v
#include <utility>              // forward

namespace dctl::core {
namespace meta {

template<auto N>
using int_c = std::integral_constant<decltype(N), N>;

template<auto... Ns>
using tuple_c = std::tuple<int_c<Ns>...>;

struct array;

struct comma;

struct plus
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return (... + std::forward<Args>(args));
        }
};

struct logical_or
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return (... || std::forward<Args>(args));
        }
};

struct bit_or
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return (... | std::forward<Args>(args));
        }
};

template<class List, class Reduce>
struct map_reduce;

template<template<class...> class List, class... Elements>
struct map_reduce<List<Elements...>, array>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction map) const
        {
                return std::experimental::make_array(map(Elements{})...);
        }
};

template<template<class...> class List, class... Elements>
struct map_reduce<List<Elements...>, comma>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction map) const
        {
                (... , map(Elements{}));
        }
};

template<template<class...> class List, class... Elements, class Reduce>
struct map_reduce<List<Elements...>, Reduce>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction map) const
        {
                return Reduce{}(map(Elements{})...);
        }
};

template<class T, class... Cases>
struct switch_;

template<class Key, class Value>
struct case_;

template<class Value>
struct default_;

template<class T, class Key, class True, class Value>
struct switch_<T, case_<Key, True>, default_<Value>>
:
        std::conditional_t<std::is_same_v<T, Key>, True, Value>
{};

template<class T, class Key, class True, class _, class False>
struct switch_<T, case_<Key, True>, case_<_, False>>
:
        std::conditional_t<std::is_same_v<T, Key>, True, False>
{};

template<class T, class Key, class True, class Head, class... Tails>
struct switch_<T, case_<Key, True>, Head, Tails...>
:
        std::conditional_t<std::is_same_v<T, Key>, True, switch_<T, Head, Tails...>>
{};

}       // namespace meta
}       // namespace dctl::core
