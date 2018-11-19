#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/array>   // make_array

namespace dctl::core {
namespace meta {

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
