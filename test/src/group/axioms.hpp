#pragma once
#include <group/primitives.hpp>         // set, op, id, inv
#include <algorithm>                    // all_of, find, find_if
#include <iterator>                     // begin, end

namespace dctl {
namespace group {
namespace axioms {

template<class Group>
auto is_closure(Group const& g) noexcept
{
        auto const first = begin(group::set(g));
        auto const last = end(group::set(g));
        auto const op = group::op(g);

        return std::all_of(first, last, [&](auto const& a) {
                return std::all_of(first, last, [&](auto const& b){
                        return last != std::find(first, last, op(a, b));
                });
        });
}

template<class Group>
auto is_associativity(Group const& g) noexcept
{
        auto const first = std::begin(group::set(g));
        auto const last = std::end(group::set(g));
        auto const op = group::op(g);

        return std::all_of(first, last, [&](auto const& a) {
                return std::all_of(first, last, [&](auto const& b){
                        return std::all_of(first, last, [&](auto const& c) {
                                return (
                                        op(a, op(b, c)) ==
                                        op(op(a, b), c)
                                );
                        });
                });
        });
}

template<class Group>
auto is_identity(Group const& g) noexcept
{
        auto const first = std::begin(group::set(g));
        auto const last = std::end(group::set(g));
        auto const op = group::op(g);

        return last != std::find_if(first, last, [&](auto const& id) {
                return std::all_of(first, last, [&](auto const& elem){
                        return (
                                op(elem, id) == elem &&
                                op(id, elem) == elem
                        );
                });
        });
}

template<class Group>
auto is_inverse(Group const& g) noexcept
{
        auto const first = begin(group::set(g));
        auto const last = end(group::set(g));
        auto const op = group::op(g);
        auto const id = group::id(g);
        auto const inv = group::inv(g);

        return std::all_of(first, last, [&](auto const& elem) {
                return last != std::find_if(first, last, [&](auto const& elem_inv){
                        return (
                                inv(elem) == elem_inv &&
                                op(elem, elem_inv) == id &&
                                op(elem_inv, elem) == id
                        );
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
}       // namespace dctl
