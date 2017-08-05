#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>              // size_t
#include <experimental/array>   // make_array
#include <type_traits>          // conditional_t, integral_constant, is_same_v
#include <utility>              // forward

namespace dctl::core {
namespace meta {

template<class... T>
struct list {};

template<class T, T N>
using integral_c = std::integral_constant<T, N>;

template<class T, T... Ns>
using list_c = list<integral_c<T, Ns>...>;

template<class L>
struct size_impl;

template<class L>
using size = typename size_impl<L>::type;

template<template<class...> class L, class... T>
struct size_impl<L<T...>>
{
        using type = std::integral_constant<std::size_t, sizeof...(T)>;
};

template<class C, class T, class F>
using if_ = std::conditional_t<static_cast<bool>(C::value), T, F>;

template<class T, class... C>
struct switch_impl;

template<class T, class... C>
using switch_ = typename switch_impl<T, C...>::type;

template<class K, class V>
struct case_;

template<class T, class K, class V, class... Cr>
struct switch_impl<T, case_<K, V>, Cr...>
{
        using type = std::conditional_t<std::is_same_v<T, K>, V, switch_<T, Cr...>>;
};

template<class V>
struct default_;

template<class T, class V, class... Cr>
struct switch_impl<T, default_<V>, Cr...>
{
        using type = V;
        static_assert(sizeof...(Cr) == 0, "default_ must be final clause inside switch_");
};

struct nonematch;

template<class T>
struct switch_impl<T>
{
        using type = nonematch;
};

template<template<class> class F, class L>
struct transform_impl;

template<template<class> class F, class L>
using transform = typename transform_impl<F, L>::type;

template<template<class> class F, template<class...> class L, class... T>
struct transform_impl<F, L<T...>>
{
        using type = L<F<T>...>;
};

template<class... L>
struct append_impl;

template<class... L>
using append = typename append_impl<L...>::type;

template<template<class...> class L, class... T>
struct append_impl<L<T...>>
{
        using type = L<T...>;
};

template<template<class...> class L, class... T1, class... T2, class... Lr>
struct append_impl<L<T1...>, L<T2...>, Lr...>
{
        using type = append<L<T1..., T2...>, Lr...>;
};

template<class L, template<class> class P>
struct remove_if_impl;

template<class L, template<class> class P>
using remove_if = typename remove_if_impl<L, P>::type;

template<class L, class Q>
using remove_if_q = remove_if<L, Q::template fn>;

template<template<class...> class L, class... T, template<class> class P>
struct remove_if_impl<L<T...>, P>
{
        using type = append<L<>, if_<P<T>, L<>, L<T>>...>;
};

template<template<class...> class F, class... T>
class bind_back
{
        template<class... U>
        struct fn_impl { using type = F<U..., T...>; };
public:
        template<class... U>
        using fn = typename fn_impl<U...>::type;
};

template<class List>
struct foldl_logical_or;

template<template<class...> class List, class... Elements>
struct foldl_logical_or<List<Elements...>>
{
        template<class UnaryFunction>
        constexpr auto operator()(UnaryFunction fun) const
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
        constexpr auto operator()(UnaryFunction fun) const
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
        constexpr auto operator()(UnaryFunction fun) const
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
        constexpr auto operator()(UnaryFunction fun) const
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
        constexpr auto operator()(UnaryFunction fun) const
        {
                return std::experimental::make_array(fun(Elements{})...);
        }
};

}       // namespace meta
}       // namespace dctl::core
