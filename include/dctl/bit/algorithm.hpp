#pragma once
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set>
auto set_none_of(Set const& s) noexcept
{
        return s.none();
}

template<class Set>
auto set_any_of(Set const& s) noexcept
{
        return s.any();
}

template<class Set>
auto set_all_of(Set const& s) noexcept
{
        return s.all();
}

template<class Set>
auto set_count(Set const& s) noexcept
{
        return s.count();
}

template<class Set, class Pred>
auto set_count_until(Set const& s, Pred pred) noexcept
{
        return s.count_until(pred);
}

template<class Set>
auto set_single(Set const& s) noexcept
{
        return bit::set_count_until(s, [](auto sum) {
                return sum > 1;
        }) == 1;
}

template<class Set>
auto set_double(Set const& s) noexcept
{
        return bit::set_count_until(s, [](auto sum) {
                return sum > 2;
        }) == 2;
}

template<class Set>
auto set_multiple(Set const& s) noexcept
{
        return bit::set_count_until(s, [](auto sum) {
                return sum > 2;
        }) > 2;
}

template<class Set>
auto set_min_element(Set const& s) noexcept
{
        return s.begin();
}

template<class Set>
auto set_max_element(Set const& s) noexcept
{
        return s.rbegin().base();
}

template<class Set>
auto set_minmax_element(Set const& s) noexcept
{
        return std::make_pair(set_min_element(s), set_max_element(s));
}

template<class Set>
auto set_equal(Set const& lhs, Set const& rhs) noexcept
{
        return lhs == rhs;
}

template<class Set>
auto set_lexicographical_compare(Set const& lhs, Set const& rhs) noexcept
{
        return lhs < rhs;
}

template<class Set>
auto set_includes(Set const& lhs, Set const& rhs) noexcept
{
        return lhs.includes(rhs);
}

template<class Set>
auto set_intersects(Set const& lhs, Set const& rhs) noexcept
{
        return lhs.intersects(rhs);
}

template<class Set>
auto set_complement(Set const& lhs) noexcept
{
        return ~lhs;
}

template<class Set>
auto set_intersection(Set const& lhs, Set const& rhs) noexcept
{
        return lhs & rhs;
}

template<class Set>
auto set_union(Set const& lhs, Set const& rhs) noexcept
{
        return lhs | rhs;
}

template<class Set>
auto set_symmetric_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs ^ rhs;
}

template<class Set>
auto set_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs & ~rhs;
}

template<class Set>
auto set_transform_plus(Set const& s, int n) noexcept
{
        return s << n;
}

template<class Set>
auto set_transform_minus(Set const& s, int n) noexcept
{
        return s >> n;
}

}       // namespace bit
}       // namespace dctl
