#pragma once

namespace xstd {

template<class Set>
constexpr bool set_single(Set const& s) noexcept
{
        return s.count() == 1;
}

template<class Set>
constexpr bool set_double(Set const& s) noexcept
{
        return s.count() == 2;
}

template<class Set>
constexpr bool set_multiple(Set const& s) noexcept
{
        return s.count() >= 2;
}

}       // namespace xstd
