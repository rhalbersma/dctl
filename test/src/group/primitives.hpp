#pragma once
#include <initializer_list>     // initializer_list
#include <tuple>                // make_tuple, get
#include <vector>               // vector

namespace dctl {

template<class Set, class Op, class Inv, class Element = typename Set::value_type>
auto make_group(Set elements, Op op, Inv inv, Element id = Element{})
{
        return std::make_tuple(elements, op, inv, id);
}

template<class Element, class Op, class Inv>
auto make_group(std::initializer_list<Element> elements, Op op, Inv inv, Element id = Element{})
{
        return std::make_tuple(std::vector<Element>{elements}, op, inv, id);
}

namespace group {

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
