#pragma once
#include <set>                          // set
#include <tuple>                        // make_tuple, get

namespace dctl {
namespace group {

template<class Set, class Op, class Inv, class Element = typename Set::value_type>
auto make(Set set, Op op, Inv inv, Element id = Element{})
{
        return std::make_tuple(set, op, inv, id);
}

template<class Group>
constexpr auto const& set(Group const& g) noexcept
{
        return std::get<0>(g);
}

template<class Group>
constexpr auto const& op(Group const& g) noexcept
{
        return std::get<1>(g);
}

template<class Group>
constexpr auto const& inv(Group const& g) noexcept
{
        return std::get<2>(g);
}

template<class Group>
constexpr auto const& id(Group const& g) noexcept
{
        return std::get<3>(g);
}

}       // namespace group
}       // namespace dctl
