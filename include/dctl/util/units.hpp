#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl {
inline namespace literals {
inline namespace iec_literals {

constexpr auto operator"" _KiB(unsigned long long n) noexcept
{
        return n << 10;
}

constexpr auto operator"" _MiB(unsigned long long n) noexcept
{
        return n << 20;
}

constexpr auto operator"" _GiB(unsigned long long n) noexcept
{
        return n << 30;
}

}       // namespace iec_literals
}       // namespace literals
}       // namespace dctl
