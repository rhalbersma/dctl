#pragma once

namespace dctl {
namespace packed {

template<typename Set>
Set set_intersection(Set const& lhs, Set const& rhs)
{
        return lhs & rhs;
}

template<typename Set>
Set set_union(Set const& lhs, Set const& rhs)
{
        return lhs | rhs;
}

template<typename Set>
Set set_symmetric_difference(Set const& lhs, Set const& rhs)
{
        return lhs ^ rhs;
}

template<typename Set>
Set set_difference(Set const& lhs, Set const& rhs)
{
        return lhs & ~rhs;
}

template<typename Set>
bool includes(Set const& lhs, Set const& rhs)
{
        return set_difference(rhs, lhs).empty();
}

}       // namespace packed
}       // namespace dctl
