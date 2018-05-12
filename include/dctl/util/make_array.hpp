#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <type_traits>

namespace xstd::experimental {
namespace detail {

template<class> 
struct is_ref_wrapper 
: 
        std::false_type 
{};

template<class T> 
struct is_ref_wrapper<std::reference_wrapper<T>> 
: 
        std::true_type 
{};

template<class T>
using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

template<class D, class...> 
struct return_type_helper 
{ 
        using type = D; 
};

template<class... Types>
struct return_type_helper<void, Types...> 
: 
        std::common_type<Types...> 
{
        static_assert
        (
                std::conjunction_v<not_ref_wrapper<Types>...>,
                "Types cannot contain reference_wrappers when D is void"
        );
};
 
template<class D, class... Types>
using return_type = std::array<typename return_type_helper<D, Types...>::type, sizeof...(Types)>;

}       // namespace detail
 
template<class D = void, class... Types>
constexpr detail::return_type<D, Types...> make_array(Types&&... t) 
{
        return { std::forward<Types>(t)... };
}

}       // namespace xstd::experimental
