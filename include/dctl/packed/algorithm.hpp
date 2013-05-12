#pragma once

namespace dctl {
namespace packed {

template<typename Set>
void swap(Set& lhs, Set& rhs)
{
        lhs.swap(rhs);
}

template<typename Set>
auto begin(Set const& s) -> decltype(s.begin())
{
        return s.begin();
}

template<typename Set>
auto end(Set const& s) -> decltype(s.end())
{
        return s.end();
}

template<typename Set>
Set operator~(Set const& lhs)
{
        return Set(lhs).flip();
}

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
