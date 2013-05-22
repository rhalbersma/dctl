#pragma once
#include <dctl/packed/raw.hpp>

namespace dctl {
namespace packed {

template<class Set>
auto set_complement(Set const& set) -> decltype(~set)
{
        return ~set;
}

template<class Set>
auto set_intersection(Set const& lhs, Set const& rhs) -> decltype(lhs & rhs)
{
        return lhs & rhs;
}

template<class Set>
auto set_union(Set const& lhs, Set const& rhs) -> decltype(lhs | rhs)
{
        return lhs | rhs;
}

template<class Set>
auto set_symmetric_difference(Set const& lhs, Set const& rhs) -> decltype(lhs ^ rhs)
{
        return lhs ^ rhs;
}

template<class Set>
auto set_difference(Set const& lhs, Set const& rhs) -> decltype(set_intersection(lhs, set_complement(rhs)))
{
        return set_intersection(lhs, set_complement(rhs));
}

template<class Set>
auto set_exclusive(Set const& lhs, Set const& rhs) -> decltype(empty(set_intersection(lhs, rhs)))
{
        return empty(set_intersection(lhs, rhs));
}

template<class Set>
auto set_includes(Set const& lhs, Set const& rhs) -> decltype(empty(set_difference(rhs, lhs)))
{
        return empty(set_difference(rhs, lhs));
}

}       // namespace packed
}       // namespace dctl
