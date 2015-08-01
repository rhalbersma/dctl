#pragma once
#include <dctl/board/angle/detail/abs_remainder.hpp>  // abs_remainder
#include <cassert>                              // assert

namespace dctl {

class Angle
{
        int degrees;

        constexpr auto make_angle(int n) const noexcept
        {
                return detail::abs_remainder(n, 360);
        }

        constexpr auto set_angle(int n) noexcept
        {
                degrees = make_angle(n);
        }

public:
        Angle() = default;

        explicit constexpr Angle(int n) noexcept
        :
                degrees{make_angle(n)}
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

}       // namespace dctl
