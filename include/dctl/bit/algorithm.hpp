#pragma once
#include <dctl/bit/raw.hpp>

namespace dctl {
namespace bit {

template<class Set>
Set set_complement(Set const& lhs)
{
        return ~lhs;
}

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
        return set_intersection(lhs, set_complement(rhs));
}

template<class Set>
bool set_exclusive(Set const& lhs, Set const& rhs)
{
        return empty(set_intersection(lhs, rhs));
}

template<class Set>
bool set_includes(Set const& lhs, Set const& rhs)
{
        return empty(set_difference(rhs, lhs));
}

}       // namespace bit
}       // namespace dctl
