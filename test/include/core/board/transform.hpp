#pragma once
#include <type_traits>  // is_same

namespace dctl::core {
namespace board {
namespace traits {
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
constexpr auto is_identity_v = std::is_same_v<
        detail::iterate_t<UnaryMetaFunction, Arg, 1>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_involution_v = std::is_same_v<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 0>
>;

template<template<class> class UnaryMetaFunction, class Arg>
constexpr auto is_idempotent_v = std::is_same_v<
        detail::iterate_t<UnaryMetaFunction, Arg, 2>,
        detail::iterate_t<UnaryMetaFunction, Arg, 1>
>;

}       // namespace traits
}       // namespace board
}       // namespace dctl::core
