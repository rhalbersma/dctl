#pragma once
#include <group/primitives.hpp>         // set, op, id
#include <algorithm>                    // all_of
#include <iterator>                     // begin, end

namespace dctl {
namespace group {
namespace action {

template<class Object, class Group>
auto is_associativity(Object const& obj, Group const& g) noexcept
{
        auto const first = begin(group::set(g));
        auto const last = end(group::set(g));
        auto const op = group::op(g);

        return std::all_of(first, last, [&](auto const& a) {
                return std::all_of(first, last, [&](auto const& b){
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

}       // namespace action
}       // namespace group
}       // namespace dctl
