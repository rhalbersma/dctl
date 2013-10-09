#pragma once
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set>
constexpr auto set_none(Set const& lhs) noexcept
{
        return lhs.none();
}

template<class Set>
constexpr auto set_any(Set const& lhs) noexcept
{
        return lhs.any();
}

template<class Set>
constexpr auto set_all(Set const& lhs) noexcept
{
        return lhs.all();
}

template<class Set>
constexpr auto set_is_count_equal_to(Set const& lhs, int n) noexcept
{
        return lhs.is_count_equal_to(n);
}

template<class Set>
constexpr auto set_is_count_less(Set const& lhs, int n) noexcept
{
        return lhs.is_count_less(n);
}

template<class Set>
constexpr auto set_count(Set const& lhs) noexcept
{
        return lhs.count();
}

template<class Set>
constexpr auto set_min_element(Set const& lhs) noexcept
{
        return lhs.begin();
}

template<class Set>
/* constexpr */ auto set_max_element(Set const& lhs) noexcept
{
        return lhs.rbegin().base();
}

template<class Set>
/* constexpr */ auto set_minmax_element(Set const& lhs) noexcept
{
        return std::make_pair(set_min_element(lhs), set_max_element(lhs));
}

template<class Set>
constexpr auto set_equal(Set const& lhs, Set const& rhs) noexcept
{
        return lhs == rhs;
}

template<class Set>
constexpr auto set_lexicographical_compare(Set const& lhs, Set const& rhs) noexcept
{
        return lhs < rhs;
}

template<class Set>
constexpr auto set_includes(Set const& lhs, Set const& rhs) noexcept
{
        return lhs.includes(rhs);
}

template<class Set>
constexpr auto set_exclusive(Set const& lhs, Set const& rhs) noexcept
{
        return !lhs.intersects(rhs);
}

template<class Set>
constexpr auto set_complement(Set const& lhs) noexcept
{
        return ~lhs;
}

template<class Set>
constexpr auto set_intersection(Set const& lhs, Set const& rhs) noexcept
{
        return lhs & rhs;
}

template<class Set>
constexpr auto set_union(Set const& lhs, Set const& rhs) noexcept
{
        return lhs | rhs;
}

template<class Set>
constexpr auto set_symmetric_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs ^ rhs;
}

template<class Set>
constexpr auto set_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs & ~rhs;
}

template<class Set>
constexpr auto set_transform_plus(Set const& lhs, int n) noexcept
{
        return lhs << n;
}

template<class Set>
constexpr auto set_transform_minus(Set const& lhs, int n) noexcept
{
        return lhs >> n;
}

}       // namespace bit
}       // namespace dctl
