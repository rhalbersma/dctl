#pragma once

namespace dctl {
inline namespace literals {
inline namespace iec_literals {

inline
constexpr auto operator"" _KiB(unsigned long long n) noexcept
{
        return n << 10;
}

inline
constexpr auto operator"" _MiB(unsigned long long n) noexcept
{
        return n << 20;
}

inline
constexpr auto operator"" _GiB(unsigned long long n) noexcept
{
        return n << 30;
}

}       // namespace iec_literals
}       // namespace literals
}       // namespace dctl
