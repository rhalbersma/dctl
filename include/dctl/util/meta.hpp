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

struct always;
struct never;

template<class Key, class Value>
struct case_;

template<class Value>
using default_ = case_<always, Value>;

template<class T, class Key>
constexpr auto match = std::is_same_v<T, Key>;

template<class T> constexpr auto match<T, always> = true;
template<class T> constexpr auto match<always, T> = true;

template<class T> constexpr auto match<T, never> = false;
template<class T> constexpr auto match<never, T> = false;

template<class T, class... Cases>
struct switch_;

template<class T, class... Cases>
using switch_t = typename switch_<T, Cases...>::type;

template<class T, class Key, class Value>
struct switch_<T, case_<Key, Value>>
:
        std::conditional<match<T, Key>, Value, never>
{};

template<class T, class Key, class Value, class _, class Other>
struct switch_<T, case_<Key, Value>, case_<_, Other>>
:
        std::conditional<match<T, Key>, Value, Other>
{};

template<class T, class Key, class Value, class Head, class... Tails>
struct switch_<T, case_<Key, Value>, Head, Tails...>
:
        std::conditional<match<T, Key>, Value, switch_t<T, Head, Tails...>>
{};

}       // namespace meta
}       // namespace dctl::core
