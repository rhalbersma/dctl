#pragma once
#include <cstddef>      // size_t
#include <type_traits>  // is_same

namespace dctl {
namespace board {
namespace traits {
namespace detail {

template<template<class> class UnaryMetaFunction, class Arg, std::size_t N>
struct iterate;

template<template<class> class UnaryMetaFunction, class Arg, std::size_t N>
using iterate_t = typename iterate<UnaryMetaFunction, Arg, N>::type;

template<template<class> class UnaryMetaFunction, class Arg>
struct iterate<UnaryMetaFunction, Arg, 0> : Arg {};

template<template<class> class UnaryMetaFunction, class Arg, std::size_t N>
struct iterate : UnaryMetaFunction<iterate_t<UnaryMetaFunction, Arg, N - 1>> {};

}       // namespace detail

template<template<class> class UnaryMetaFunction, class Arg>
using is_identity = std::is_same<
        detail::iterate_t<UnaryMetaFunction, Arg, 1>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_identity_v = is_identity<UnaryMetaFunction, Arg>::value;

template<template<class> class UnaryMetaFunction, class Arg>
using is_involution = std::is_same<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_involution_v = is_involution<UnaryMetaFunction, Arg>::value;

template<template<class> class UnaryMetaFunction, class Arg>
using is_idempotent = std::is_same<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 1>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_idempotent_v = is_idempotent<UnaryMetaFunction, Arg>::value;

}       // namespace traits
}       // namespace board
}       // namespace dctl
