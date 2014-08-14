#pragma once
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set, class Size>
Set transform_plus_n(Set const& s, Size n) noexcept
{
        return s << n;
}

template<class Set, class Size>
Set transform_minus_n(Set const& s, Size n) noexcept
{
        return s >> n;
}

template<class Set, class Pred>
auto count_until(Set const& s, Pred pred) noexcept
{
        return s.count_until(pred);
}

template<class Set>
auto set_single(Set const& s) noexcept
{
        return count_until(s, [](auto sum) {
                return sum > 1;
        }) == 1;
}

template<class Set>
auto set_double(Set const& s) noexcept
{
        return count_until(s, [](auto sum) {
                return sum > 2;
        }) == 2;
}

template<class Set>
auto set_multiple(Set const& s) noexcept
{
        return count_until(s, [](auto sum) {
                return sum > 2;
        }) > 2;
}

template<class Set>
constexpr Set set_complement(Set const& lhs) noexcept
{
        return ~lhs;
}

template<class Set>
constexpr Set set_union(Set const& lhs, Set const& rhs) noexcept
{
        return lhs | rhs;
}

template<class Set>
constexpr Set set_intersection(Set const& lhs, Set const& rhs) noexcept
{
        return lhs & rhs;
}

template<class Set>
constexpr Set set_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs - rhs;
}

template<class Set>
constexpr Set set_symmetric_difference(Set const& lhs, Set const& rhs) noexcept
{
        return lhs ^ rhs;
}

template<class Set>
auto min_element(Set const& s) noexcept
{
        return s.begin();
}

template<class Set>
auto max_element(Set const& s) noexcept
{
        return s.rbegin().base();
}

template<class Set>
auto minmax_element(Set const& s) noexcept
{
        return std::make_pair(set_min_element(s), set_max_element(s));
}

template<class Set>
bool colexicographical_compare(Set const& lhs, Set const& rhs) noexcept
{
        return lhs < rhs;
}

}       // namespace bit
}       // namespace dctl
