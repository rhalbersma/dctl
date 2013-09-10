#pragma once
#include <tuple>                        // get, make_tuple, tuple

namespace dctl {
namespace group {

template<class Set, class Op, class Id, class Inv>
using Group = std::tuple<Set, Op, Id, Inv>;

template<class Set, class Op, class Id, class Inv>
constexpr auto make_group(Set const& elements, Op op, Id id, Inv inv)
{
        return std::make_tuple(elements, op, id, inv);
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
constexpr auto const& id(Group const& g) noexcept
{
        return std::get<2>(g);
}

template<class Group>
constexpr auto const& inv(Group const& g) noexcept
{
        return std::get<3>(g);
}

}       // namespace group
}       // namespace dctl
