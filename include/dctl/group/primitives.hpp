#pragma once
#include <tuple>                        // tuple

namespace dctl {
namespace group {

template<class Set, class Op, class Id, class Inv>
using Group = std::tuple<Set, Op, Id, Inv>;

template<class Group>
constexpr auto set(Group const& group) noexcept
{
        return std::get<0>(group);
}

template<class Group>
constexpr auto op(Group const& group) noexcept
{
        return std::get<1>(group);
}

template<class Group>
constexpr auto id(Group const& group) noexcept
{
        return std::get<2>(group);
}

template<class Group>
constexpr auto inv(Group const& group) noexcept
{
        return std::get<3>(group);
}

}       // namespace group
}       // namespace dctl
