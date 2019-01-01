#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/group/primitives.hpp>      // set, op, id, inv
#include <algorithm>                            // all_of, find, find_if

namespace dctl::core {
namespace group {
namespace axioms {

template<class Group>
auto is_closure(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return std::all_of(set.begin(), set.end(), [&](auto const& a) {
                return std::all_of(set.begin(), set.end(), [&](auto const& b) {
                        return set.cend() != std::find(set.begin(), set.end(), op(a, b));
                });
        });
}

template<class Group>
auto is_associativity(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return std::all_of(set.begin(), set.end(), [&](auto const& a) {
                return std::all_of(set.begin(), set.end(), [&](auto const& b) {
                        return std::all_of(set.begin(), set.end(), [&](auto const& c) {
                                return
                                        op(a, op(b, c)) ==
                                        op(op(a, b), c)
                                ;
                        });
                });
        });
}

template<class Group>
auto is_identity(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return set.cend() != std::find_if(set.begin(), set.end(), [&](auto const& id) {
                return std::all_of(set.begin(), set.end(), [&](auto const& elem) {
                        return
                                op(elem, id) == elem &&
                                op(id, elem) == elem
                        ;
                });
        });
}

template<class Group>
auto is_inverse(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);
        auto const id = group::id(g);
        auto const inv = group::inv(g);

        return std::all_of(set.begin(), set.end(), [&](auto const& elem) {
                return set.cend() != std::find_if(set.begin(), set.end(), [&](auto const& elem_inv) {
                        return
                                inv(elem) == elem_inv &&
                                op(elem, elem_inv) == id &&
                                op(elem_inv, elem) == id
                        ;
                });
        });
}

template<class Group>
auto is_realized(Group const& g) noexcept
{
        return
                group::axioms::is_closure(g) &&
                group::axioms::is_associativity(g) &&
                group::axioms::is_identity(g) &&
                group::axioms::is_inverse(g)
        ;
}

}       // namespace axioms
}       // namespace group
}       // namespace dctl::core
