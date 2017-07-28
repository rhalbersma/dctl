#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/array>   // make_array
#include <tuple>                // tuple
#include <type_traits>          // integral_constant
#include <utility>              // forward

namespace dctl::core {
namespace meta {

template<template<class...> class MetaFunction>
struct quote
{
        template<class... Args>
        struct apply
        :
                MetaFunction<Args...>
        {};
};

template<class MetaFunctionClass, class... Args>
struct apply
:
        MetaFunctionClass::template apply<Args...>
{};

template<class TypeList, class MetaFunctionClass>
struct foldr_comma;

template<class... Elements, class MetaFunctionClass>
struct foldr_comma<std::tuple<Elements...>, MetaFunctionClass>
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return (apply<MetaFunctionClass, Elements>{}(std::forward<Args>(args)...) , ...);
        }
};

template<class TypeList, class MetaFunctionClass>
struct foldr_bitor;

template<class... Elements, class MetaFunctionClass>
struct foldr_bitor<std::tuple<Elements...>, MetaFunctionClass>
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return (apply<MetaFunctionClass, Elements>{}(std::forward<Args>(args)...) | ...);
        }
};

template<class TypeList, class MetaFunctionClass>
struct make_array;

template<class... Elements, class MetaFunctionClass>
struct make_array<std::tuple<Elements...>, MetaFunctionClass>
{
        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return std::experimental::make_array(apply<MetaFunctionClass, Elements>{}(std::forward<Args>(args)...)...);
        }
};

template<int N>
using int_c = std::integral_constant<int, N>;

}       // namespace meta
}       // namespace dctl::core
