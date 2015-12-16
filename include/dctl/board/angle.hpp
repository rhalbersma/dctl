#pragma once
#include <xstd/cstdlib.hpp>     // euclidean_div
#include <cassert>              // assert
#include <type_traits>          // integral_constant

namespace dctl {

class Angle
{
        int degrees;

        constexpr auto as_angle(int n) const noexcept
        {
                return xstd::euclidean_div(n, 360).rem;
        }

        constexpr auto set_angle(int n) noexcept
        {
                degrees = as_angle(n);
        }

public:
        Angle() = default;

        explicit constexpr Angle(int n) noexcept
        :
                degrees{as_angle(n)}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return degrees;
        }

        constexpr auto& operator+=(Angle const& other) noexcept
        {
                set_angle(degrees + other.degrees);
                return *this;
        }

        constexpr auto& operator-=(Angle const& other) noexcept
        {
                set_angle(degrees - other.degrees);
                return *this;
        }

        constexpr auto& operator*=(int n) noexcept
        {
                set_angle(degrees * n);
                return *this;
        }

        constexpr auto& operator/=(int n) // Throws: Nothing.
        {
                assert(n != 0);
                set_angle(degrees / n);
                return *this;
        }

        friend constexpr auto operator+(Angle const& a) noexcept
        {
                return Angle{+a.degrees};
        }

        friend constexpr auto operator-(Angle const& a) noexcept
        {
                return Angle{-a.degrees};
        }
};

inline namespace literals {
inline namespace angle_literals {

constexpr auto operator"" _deg(unsigned long long n) noexcept
{
        return Angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals

constexpr auto operator+(Angle const& lhs, Angle const& rhs) noexcept
{
        Angle nrv{lhs}; nrv += rhs; return nrv;
}

constexpr auto operator-(Angle const& lhs, Angle const& rhs) noexcept
{
        Angle nrv{lhs}; nrv -= rhs; return nrv;
}

constexpr auto operator*(Angle const& a, int n) noexcept
{
        Angle nrv{a}; nrv *= n; return nrv;
}

constexpr auto operator*(int n, Angle const& a) noexcept
{
        Angle nrv{a}; nrv *= n; return nrv;
}

constexpr auto operator/(Angle const& a, int n) // Throws: Nothing.
{
        assert(n != 0);
        Angle nrv{a}; nrv /= n; return nrv;
}

constexpr auto inverse(Angle const& a) noexcept
{
        return -a;
}

constexpr auto rotate(Angle const& a, Angle const& b) noexcept
{
        return a + b;
}

constexpr auto mirror(Angle const& a, Angle const& b) noexcept
{
        // a | rotate(inverse(b)) | inverse() | rotate(b)
        return rotate(inverse(rotate(a, inverse(b))), b);
}

/*

                   up
                   |
          left_up  |  right_up
                 \ | /
                  \|/
         left ----- ----- right
                  /|\
                 / | \
        left_down  |  right_down
                   |
                  down

*/

constexpr auto right(Angle const& a) noexcept
{
        return rotate(0_deg, a);
}

constexpr auto right_up(Angle const& a) noexcept
{
        return rotate(45_deg, a);
}

constexpr auto up(Angle const& a) noexcept
{
        return rotate(90_deg, a);
}

constexpr auto left_up(Angle const& a) noexcept
{
        return rotate(135_deg, a);
}

constexpr auto left(Angle const& a) noexcept
{
        return rotate(180_deg, a);
}

constexpr auto left_down(Angle const& a) noexcept
{
        return rotate(225_deg, a);
}

constexpr auto down(Angle const& a) noexcept
{
        return rotate(270_deg, a);
}

constexpr auto right_down(Angle const& a) noexcept
{
        return rotate(315_deg, a);
}

/*

                 90
                 |
            135  |  45
               \ | /
                \|/
        180 ----- ----- 0
                /|\
               / | \
            225  |  315
                 |
                270

*/

constexpr auto is_orthogonal(Angle const& a) noexcept
{
        return a % 90_deg == 0_deg;
}

constexpr auto is_diagonal(Angle const& a) noexcept
{
        return a % 90_deg == 45_deg;
}

constexpr auto is_up(Angle const& a) noexcept
{
        return 0_deg < a && a < 180_deg;
}

constexpr auto is_down(Angle const& a) noexcept
{
        return 180_deg < a;
}

constexpr auto is_left(Angle const& a) noexcept
{
        return 90_deg < a && a < 270_deg;
}

constexpr auto is_right(Angle const& a) noexcept
{
        return 270_deg < a || a < 90_deg;
}

constexpr auto is_positive(Angle const& a) noexcept
{
        return 0_deg < a && a <= 180_deg;
}

constexpr auto is_negative(Angle const& a) noexcept
{
        return a == 0_deg || 180_deg < a;
}

template<int N>
using angle_constant = std::integral_constant<int, Angle{N}>;

template<int N>
static constexpr auto angle_v = angle_constant<N>::value;

}       // namespace dctl
