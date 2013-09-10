#pragma once
#include <algorithm>                    // all_of, find, find_if
#include <iterator>                     // begin, end
#include <dctl/group/primitives.hpp>    // set, op, id

namespace dctl {
namespace group {
namespace axioms {

template<class Group>
auto is_closure(Group const& g) noexcept
{
        auto const first = std::begin(group::set(g));
        auto const last = std::end(group::set(g));
        auto const op = group::op(g);
        using Element = decltype(*first);

        return std::all_of(first, last, [=](Element const& a) {
                return std::all_of(first, last, [=](Element const& b){
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
        using Element = decltype(*first);

        return std::all_of(first, last, [=](Element const& a) {
                return std::all_of(first, last, [=](Element const& b){
                        return std::all_of(first, last, [=](Element const& c) {
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
        using Element = decltype(*first);

        return last != std::find_if(first, last, [=](Element const& id) {
                return std::all_of(first, last, [=](Element const& elem){
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
        auto const first = std::begin(group::set(g));
        auto const last = std::end(group::set(g));
        auto const op = group::op(g);
        auto const id = group::id(g);
        auto const inv = group::inv(g);
        using Element = decltype(*first);

        return std::all_of(first, last, [=](Element const& elem) {
                return last != std::find_if(first, last, [=](Element const& elem_1){
                        return (
                                inv(elem) == elem_1 &&
                                op(elem, elem_1) == id &&
                                op(elem_1, elem) == id
                        );
                });
        });
}

template<class Group>
auto is_realized(Group const& g) noexcept
{
        return (
                group::axioms::is_closure(g) &&
                group::axioms::is_associativity(g) &&
                group::axioms::is_identity(g) &&
                group::axioms::is_inverse(g)
        );
}

}       // namespace axioms
}       // namespace group
}       // namespace dctl
