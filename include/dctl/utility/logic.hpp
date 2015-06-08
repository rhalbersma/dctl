#pragma once

namespace dctl {
namespace util {

constexpr auto implies(bool p, bool q) noexcept
{
        return !p || q;
}

constexpr auto nand(bool p, bool q) noexcept
{
        return !(p && q);
}

}       // namespace util
}       // namespace dctl
