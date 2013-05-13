#pragma once

namespace dctl {
namespace packed {

template<class Set>
Set set_intersection(Set const& lhs, Set const& rhs)
{
        return lhs & rhs;
}

template<class Set>
Set set_union(Set const& lhs, Set const& rhs)
{
        return lhs | rhs;
}

template<class Set>
Set set_symmetric_difference(Set const& lhs, Set const& rhs)
{
        return lhs ^ rhs;
}

template<class Set>
Set set_difference(Set const& lhs, Set const& rhs)
{
        return lhs & ~rhs;
}

template<class Set>
bool includes(Set const& lhs, Set const& rhs)
{
        return set_difference(rhs, lhs).empty();
}

}       // namespace packed
}       // namespace dctl
