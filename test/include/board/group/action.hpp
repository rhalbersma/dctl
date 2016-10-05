#pragma once
#include <board/group/primitives.hpp>           // set, op, id
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of

namespace dctl {
namespace board {
namespace group {
namespace action {

template<class Object, class Group>
auto is_associativity(Object const& obj, Group const& g) noexcept
{
        auto const set = group::set(g);
        auto const op = group::op(g);

        return boost::algorithm::all_of(set, [&](auto const& a) {
                return boost::algorithm::all_of(set, [&](auto const& b){
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
}       // namespace board
}       // namespace dctl
