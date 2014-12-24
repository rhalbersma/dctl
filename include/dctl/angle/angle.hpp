#pragma once
#include <dctl/angle/detail/abs_remainder.hpp>  // abs_remainder
#include <cassert>                              // assert

namespace dctl {

class Angle
{
        int degrees{};

        constexpr auto invariant() const noexcept
        {
                return 0 <= degrees && degrees < 360;
        }

        constexpr auto make_angle(int n) const noexcept
        {
                return detail::abs_remainder(n, 360);
        }

public:
        constexpr Angle() = default;

        explicit constexpr Angle(int n) noexcept
        :
                degrees{make_angle(n)}
        {
                assert(invariant());
        }

        /* implicit */ constexpr operator auto() const noexcept
        {
                return degrees;
        }

        constexpr auto& operator+=(Angle const& other) noexcept
        {
                degrees = make_angle(degrees + other.degrees);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator-=(Angle const& other) noexcept
        {
                degrees = make_angle(degrees - other.degrees);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator*=(int n) noexcept
        {
                degrees = make_angle(degrees * n);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator/=(int n) // Throws: Nothing.
        {
                assert(n != 0);
                degrees = make_angle(degrees / n);
                assert(invariant());
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

inline
constexpr auto operator+(Angle const& lhs, Angle const& rhs) noexcept
{
        Angle nrv{lhs}; nrv += rhs; return nrv;
}

inline
constexpr auto operator-(Angle const& lhs, Angle const& rhs) noexcept
{
        Angle nrv{lhs}; nrv -= rhs; return nrv;
}

inline
constexpr auto operator*(Angle const& a, int n) noexcept
{
        Angle nrv{a}; nrv *= n; return nrv;
}

inline
constexpr auto operator*(int n, Angle const& a) noexcept
{
        Angle nrv{a}; nrv *= n; return nrv;
}

inline
constexpr auto operator/(Angle const& a, int n) // Throws: Nothing.
{
        assert(n != 0);
        Angle nrv{a}; nrv /= n; return nrv;
}

}       // namespace dctl
