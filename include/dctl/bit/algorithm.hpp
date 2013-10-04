#pragma once
#include <cstddef>                      // size_t
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set>
constexpr auto set_none_of(Set const& lhs) noexcept
{
        return lhs.none();
}

template<class Set>
constexpr auto set_any_of(Set const& lhs) noexcept
{
        return lhs.any();
}

template<class Set>
constexpr auto set_all_of(Set const& lhs) noexcept
{
        return lhs.all();
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
constexpr auto set_exclusive(Set const& lhs, Set const& rhs) noexcept
{
        return set_intersection(lhs, rhs).empty();
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
constexpr auto set_includes(Set const& lhs, Set const& rhs) noexcept
{
        return set_difference(rhs, lhs).empty();
}

template<class Set>
constexpr auto set_transform_plus(Set const& lhs, std::size_t n) noexcept
{
        return lhs << n;
}

template<class Set>
constexpr auto set_transform_minus(Set const& lhs, std::size_t n) noexcept
{
        return lhs >> n;
}

}       // namespace bit
}       // namespace dctl
