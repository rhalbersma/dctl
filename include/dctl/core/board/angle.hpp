#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <xstd/cstdlib.hpp>     // euclidean_div
#include <cassert>              // assert

namespace dctl::core {

class angle
{
        int m_value{};
public:
        angle() = default;

        explicit constexpr angle(int n) noexcept
        :
                m_value{xstd::euclidean_div(n, 360).rem}
        {
                assert(0 <= m_value && m_value < 360);
        }

        constexpr auto value() const noexcept
        {
                return m_value;
        }
};

inline namespace literals {
inline namespace angle_literals {

constexpr auto operator"" _deg(unsigned long long n) noexcept
{
        return angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals

constexpr auto operator==(angle a, angle b) noexcept
{
        return a.value() == b.value();
}

constexpr auto operator!=(angle a, angle b) noexcept
{
        return !(a == b);
}

constexpr auto operator< (angle a, angle b) noexcept
{
        return a.value() < b.value();
}

constexpr auto operator> (angle a, angle b) noexcept
{
        return b < a;
}

constexpr auto operator>=(angle a, angle b) noexcept
{
        return !(a < b);
}

constexpr auto operator<=(angle a, angle b) noexcept
{
        return !(b < a);
}

constexpr auto operator+(angle a) noexcept
{
        return angle{+a.value()};
}

constexpr auto operator-(angle a) noexcept
{
        return angle{-a.value()};
}

constexpr auto operator+(angle a, angle b) noexcept
{
        return angle{a.value() + b.value()};
}

constexpr auto operator-(angle a, angle b) noexcept
{
        return angle{a.value() - b.value()};
}

constexpr auto operator*(angle a, int n) noexcept
{
        return angle{a.value() * n};
}

constexpr auto operator*(int n, angle a) noexcept
{
        return angle{n * a.value()};
}

constexpr auto operator/(angle a, int n) // Throws: Nothing.
{
        assert(n != 0);
        return angle{a.value() / n};
}

constexpr auto operator/(angle a, angle b) // Throws: Nothing.
{
        assert(b.value() != 0);
        return a.value() / b.value();
}

constexpr auto operator%(angle a, angle b) // Throws: Nothing.
{
        assert(b.value() != 0);
        return angle{a.value() % b.value()};
}

constexpr auto inverse(angle a) noexcept
{
        return -a;
}

constexpr auto rotate(angle a, angle b) noexcept
{
        return a + b;
}

constexpr auto reverse(angle a) noexcept
{
        return rotate(a, 180_deg);
}

constexpr auto mirror(angle a, angle b) noexcept
{
        return rotate(inverse(rotate(a, inverse(b))), b);
}

/*

                 N == 90
                   |
         135 == NW | NE == 45
                 \ | /
                  \|/
     180 == W ----- ----- E == 0
                  /|\
                 / | \
         225 == SW | SE == 315
                   |
               270 == S

*/

inline constexpr auto dir_E  =   0;
inline constexpr auto dir_NE =  45;
inline constexpr auto dir_N  =  90;
inline constexpr auto dir_NW = 135;
inline constexpr auto dir_W  = 180;
inline constexpr auto dir_SW = 225;
inline constexpr auto dir_S  = 270;
inline constexpr auto dir_SE = 315;

constexpr auto is_up(angle a) noexcept
{
        return 0_deg < a && a < 180_deg;
}

}       // namespace dctl::core
