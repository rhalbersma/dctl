#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/group/primitives.hpp>      // set, op, id
#include <algorithm>                            // all_of

namespace dctl::core {
namespace group::action {

template<class Object, class Group>
auto is_associativity(Object const& obj, Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return std::all_of(set.begin(), set.end(), [&](auto const& a) {
                return std::all_of(set.begin(), set.end(), [&](auto const& b) {
                        return op(obj, op(a, b)) == op(op(obj, a), b);
                });
        });
}

template<class Object, class Group>
auto is_identity(Object const& obj, Group const& g) noexcept
{
        auto const op = group::op(g);
        auto const id = group::id(g);

        return op(obj, id) == obj;
}

template<class Object, class Group>
auto is_realized(Object const& obj, Group const& g) noexcept
{
        return
                is_associativity(obj, g) &&
                is_identity(obj, g)
        ;
}

}       // namespace group::action
}       // namespace dctl::core
