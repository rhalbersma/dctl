#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/group/primitives.hpp>      // set, op, id, inv
#include <algorithm>                            // all_of, find, find_if

namespace dctl::core {
namespace group::axioms {

template<class Group>
auto is_closure(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return std::ranges::all_of(set, [&](auto const& a) {
                return std::ranges::all_of(set, [&](auto const& b) {
                        return set.cend() != std::ranges::find(set, op(a, b));
                });
        });
}

template<class Group>
auto is_associativity(Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return std::ranges::all_of(set, [&](auto const& a) {
                return std::ranges::all_of(set, [&](auto const& b) {
                        return std::ranges::all_of(set, [&](auto const& c) {
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

        return set.cend() != std::ranges::find_if(set, [&](auto const& id) {
                return std::ranges::all_of(set, [&](auto const& elem) {
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

        return std::ranges::all_of(set, [&](auto const& elem) {
                return set.cend() != std::ranges::find_if(set, [&](auto const& elem_inv) {
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

}       // namespace group::axioms
}       // namespace dctl::core
