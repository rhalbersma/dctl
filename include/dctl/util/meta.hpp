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

}       // namespace meta
}       // namespace dctl::core
