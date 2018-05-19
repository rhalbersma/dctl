#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <xstd/cstdlib.hpp>     // euclidean_div
#include <cassert>              // assert

namespace dctl::core {

class angle
{
        constexpr auto assert_invariants() const noexcept
        {
                assert(0 <= m_value); assert(m_value < 360);
        }

        int m_value{};
public:
        angle() = default;

        explicit constexpr angle(int const n) noexcept
        :
                m_value{xstd::euclidean_div(n, 360).rem}
        {
                assert_invariants();
        }

        constexpr auto value() const noexcept
        {
                return m_value;
        }
};

inline namespace literals {
inline namespace angle_literals {

constexpr auto operator"" _deg(unsigned long long const n) noexcept
{
        return angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals

constexpr auto operator==(angle const a, angle const b) noexcept
{
        return a.value() == b.value();
}

constexpr auto operator!=(angle const a, angle const b) noexcept
{
        return !(a == b);
}

constexpr auto operator<(angle const a, angle const b) noexcept
{
        return a.value() < b.value();
}

constexpr auto operator>(angle const a, angle const b) noexcept
{
        return b < a;
}

constexpr auto operator>=(angle const a, angle const b) noexcept
{
        return !(a < b);
}

constexpr auto operator<=(angle const a, angle const b) noexcept
{
        return !(b < a);
}

constexpr auto operator+(angle const a) noexcept
{
        return angle{+a.value()};
}

constexpr auto operator-(angle const a) noexcept
{
        return angle{-a.value()};
}

constexpr auto operator+(angle const a, angle const b) noexcept
{
        return angle{a.value() + b.value()};
}

constexpr auto operator-(angle const a, angle const b) noexcept
{
        return angle{a.value() - b.value()};
}

constexpr auto operator*(angle const a, int const n) noexcept
{
        return angle{a.value() * n};
}

constexpr auto operator*(int const n, angle const a) noexcept
{
        return angle{n * a.value()};
}

constexpr auto operator/(angle const a, int const n) // Throws: Nothing.
{
        assert(n != 0);
        return angle{a.value() / n};
}

constexpr auto operator/(angle const a, angle const b) // Throws: Nothing.
{
        assert(b.value() != 0);
        return a.value() / b.value();
}

constexpr auto operator%(angle const a, angle const b) // Throws: Nothing.
{
        assert(b.value() != 0);
        return angle{a.value() % b.value()};
}

constexpr auto inverse(angle const a) noexcept
{
        return -a;
}

constexpr auto rotate(angle const a, angle const b) noexcept
{
        return a + b;
}

constexpr auto reverse(angle const a) noexcept
{
        return rotate(a, 180_deg);
}

constexpr auto mirror(angle const a, angle const b) noexcept
{
        return rotate(inverse(rotate(a, inverse(b))), b);
}

constexpr auto is_orthogonal(angle const a) noexcept
{
        return a % 90_deg == 0_deg;
}

constexpr auto is_diagonal(angle const a) noexcept
{
        return a % 90_deg == 45_deg;
}

constexpr auto is_up(angle const a) noexcept
{
        return 0_deg < a && a < 180_deg;
}

constexpr auto is_down(angle const a) noexcept
{
        return 180_deg < a;
}

constexpr auto is_left(angle const a) noexcept
{
        return 90_deg < a && a < 270_deg;
}

constexpr auto is_right(angle const a) noexcept
{
        return 270_deg < a || a < 90_deg;
}

}       // namespace dctl::core
