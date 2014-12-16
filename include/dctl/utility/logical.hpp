#pragma once

namespace dctl {

constexpr auto implies(bool p, bool q) noexcept
{
        return !p || q;
}

}       // namespace dctl
