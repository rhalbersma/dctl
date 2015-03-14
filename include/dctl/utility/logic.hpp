#pragma once

namespace dctl {
namespace util {

inline
constexpr auto implies(bool p, bool q) noexcept
{
        return !p || q;
}

inline
constexpr auto nand(bool p, bool q) noexcept
{
        return !(p && q);
}

}       // namespace util
}       // namespace dctl