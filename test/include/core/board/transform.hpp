#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <concepts>     // same_as

namespace dctl::core {
namespace detail {

template<template<class> class UnaryMetaFunction, class Arg, int N>
struct iterate;

template<template<class> class UnaryMetaFunction, class Arg, int N>
using iterate_t = typename iterate<UnaryMetaFunction, Arg, N>::type;

template<template<class> class UnaryMetaFunction, class Arg>
struct iterate<UnaryMetaFunction, Arg, 0> : Arg {};

template<template<class> class UnaryMetaFunction, class Arg, int N>
struct iterate : UnaryMetaFunction<iterate_t<UnaryMetaFunction, Arg, N - 1>> {};

}       // namespace detail

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_identity_v = std::same_as<
        detail::iterate_t<UnaryMetaFunction, Arg, 1>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_involution_v = std::same_as<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_idempotent_v = std::same_as<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 1>
>;

}       // namespace dctl::core
