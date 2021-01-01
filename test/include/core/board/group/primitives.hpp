#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/util/type_traits.hpp>    // value_t
#include <initializer_list>             // initializer_list
#include <tuple>                        // make_tuple, get
#include <vector>                       // vector

namespace dctl::core {

template<class Element, class Op, class Inv>
auto make_group(std::initializer_list<Element> elements, Op op, Inv inv, Element id = Element{})
{
        return std::make_tuple(std::vector<Element>{elements}, op, inv, id);
}

namespace group {

template<class Group>
constexpr decltype(auto) set(Group const& g) noexcept
{
        return std::get<0>(g);
}

template<class Group>
constexpr decltype(auto) op(Group const& g) noexcept
{
        return std::get<1>(g);
}

template<class Group>
constexpr decltype(auto) inv(Group const& g) noexcept
{
        return std::get<2>(g);
}

template<class Group>
constexpr decltype(auto) id(Group const& g) noexcept
{
        return std::get<3>(g);
}

}       // namespace group
}       // namespace dctl::core
