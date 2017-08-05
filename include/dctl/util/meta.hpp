#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/array>   // make_array
#include <type_traits>          // conditional, enable_if, integral_constant, is_same_v
#include <utility>              // forward

namespace dctl::core {
namespace meta {

template<class... Elements>
struct list
{
        using type = list;
};

template<class T, T N>
using integral_c = std::integral_constant<T, N>;

template<class T, T... Ns>
using list_c = list<integral_c<T, Ns>...>;

template<class List>
constexpr auto size_v = 0;

template<class... Elements>
constexpr auto size_v<list<Elements...>> = sizeof...(Elements);

template<class MetaFunctionClass, class... Args>
struct apply
:
        MetaFunctionClass::template apply<Args...>
{};

template<class MetaFunctionClass, class... Args>
using apply_t = typename apply<MetaFunctionClass, Args...>::type;

template<class... Lists> struct append;

template<class... Lists>
using append_t = typename append<Lists...>::type;

template<>
struct append<>
:
        list_c<int>
{};

template<template<class...> class List, class... Elements>
struct append<List<Elements...>>
:
        List<Elements...>
{};

template<template<class...> class List, class... Elements1, class... Elements2, class... RemainingLists>
struct append<List<Elements1...>, List<Elements2...>, RemainingLists...>
:
        append<List<Elements1..., Elements2...>, RemainingLists...>
{};

template<class List, class Value>
struct remove;

template<class List, class Value>
using remove_t = typename remove<List, Value>::type;

template<template<class...> class List, class... Elements, class Value>
struct remove<List<Elements...>, Value>
:
        append<std::conditional_t<std::is_same_v<Value, Elements>, List<>, List<Elements>>...>
{};

template<class List, class Op>
struct transform;

template<class List, class Op>
using transform_t = typename transform<List, Op>::type;

template<template<class...> class List, class... Elements, class Op>
struct transform<List<Elements...>, Op>
:
        List<apply_t<Op, Elements>...>
{};

template<class T, class... Cases>
struct switch_;

template<class T, class... Cases>
using switch_t = typename switch_<T, Cases...>::type;

template<class Key, class Value>
struct case_;

template<class T, class Key, class Value, class... RemainingCases>
struct switch_<T, case_<Key, Value>, RemainingCases...>
:
        std::conditional<std::is_same_v<T, Key>, Value, switch_t<T, RemainingCases...>>
{};

template<class Value>
struct default_;

template<class T, class Value, class... RemainingCases>
struct switch_<T, default_<Value>, RemainingCases...>
:
        std::enable_if<true, Value>
{
        static_assert(sizeof...(RemainingCases) == 0, "default_ must be final case_ inside switch_");
};

struct nonematch;

template<class T>
struct switch_<T>
:
        std::enable_if<true, nonematch>
{};

template<class List>
struct foldl_logical_or;

template<template<class...> class List, class... Elements>
struct foldl_logical_or<List<Elements...>>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                return (... || fun(Elements{}));
        }
};

template<class List>
struct foldl_plus;

template<template<class...> class List, class... Elements>
struct foldl_plus<List<Elements...>>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                return (... + fun(Elements{}));
        }
};

template<class List>
struct foldl_comma;

template<template<class...> class List, class... Elements>
struct foldl_comma<List<Elements...>>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                (... , fun(Elements{}));
        }
};

template<class List>
struct foldl_bit_or;

template<template<class...> class List, class... Elements>
struct foldl_bit_or<List<Elements...>>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                return (... | fun(Elements{}));
        }
};

template<class List>
struct make_array;

template<template<class...> class List, class... Elements>
struct make_array<List<Elements...>>
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                return std::experimental::make_array(fun(Elements{})...);
        }
};

}       // namespace meta
}       // namespace dctl::core
