#pragma once
#include <cstddef>                      // size_t

namespace dctl {
namespace bit {

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
constexpr auto set_plus(Set const& lhs, std::size_t n) noexcept
{
        return lhs << n;
}

template<class Set>
constexpr auto set_minus(Set const& lhs, std::size_t n) noexcept
{
        return lhs >> n;
}

template<class Set>
constexpr auto set_difference(Set const& lhs, Set const& rhs) noexcept
{
        return set_intersection(lhs, set_complement(rhs));
}

template<class Set>
constexpr auto set_exclusive(Set const& lhs, Set const& rhs) noexcept
{
        return set_intersection(lhs, rhs).empty();
}

template<class Set>
constexpr auto set_includes(Set const& lhs, Set const& rhs) noexcept
{
        return set_difference(rhs, lhs).empty();
}

}       // namespace bit
}       // namespace dctl
