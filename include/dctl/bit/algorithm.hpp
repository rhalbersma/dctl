#pragma once
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set>
constexpr auto set_none(Set const& s) noexcept
{
        return s.none();
}

template<class Set>
constexpr auto set_any(Set const& s) noexcept
{
        return s.any();
}

template<class Set>
constexpr auto set_all(Set const& s) noexcept
{
        return s.all();
}

template<class Set>
constexpr auto set_is_count_equal_to(Set const& s, int n) noexcept
{
        return s.is_count_equal_to(n);
}

template<class Set>
constexpr auto set_is_count_less(Set const& s, int n) noexcept
{
        return s.is_count_less(n);
}

template<class Set>
constexpr auto is_single(Set const& s) noexcept
{
        return bit::set_is_count_equal_to(s, 1);
}

template<class Set>
constexpr auto is_double(Set const& s) noexcept
{
        return bit::set_is_count_equal_to(s, 2);
}

template<class Set>
constexpr auto is_multiple(Set const& s) noexcept
{
        return !bit::set_is_count_less(s, 2);
}

template<class Set>
constexpr auto set_count(Set const& s) noexcept
{
        return s.count();
}

template<class Set>
constexpr auto set_min_element(Set const& s) noexcept
{
        return s.begin();
}

template<class Set>
/* constexpr */ auto set_max_element(Set const& s) noexcept
{
        return s.rbegin().base();
}

template<class Set>
/* constexpr */ auto set_minmax_element(Set const& s) noexcept
{
        return std::make_pair(set_min_element(s), set_max_element(s));
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
constexpr auto set_transform_plus(Set const& s, int n) noexcept
{
        return s << n;
}

template<class Set>
constexpr auto set_transform_minus(Set const& s, int n) noexcept
{
        return s >> n;
}

}       // namespace bit
}       // namespace dctl
