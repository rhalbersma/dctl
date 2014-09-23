#pragma once
#include <dctl/angle/detail/abs_remainder.hpp>  // abs_remainder
#include <cassert>                              // assert

namespace dctl {

class Angle
{
public:
        // constructors

        explicit constexpr Angle(int n) noexcept
        :
                value_{make_angle(n)}
        {
                assert(invariant());
        }

        constexpr Angle() = default;

        // modifiers

        constexpr auto& operator+=(Angle const& other) noexcept
        {
                value_ = make_angle(value_ + other.value_);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator-=(Angle const& other) noexcept
        {
                value_ = make_angle(value_ - other.value_);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator*=(int n) noexcept
        {
                value_ = make_angle(value_ * n);
                assert(invariant());
                return *this;
        }

        constexpr auto& operator/=(int n)
        {
                assert(n != 0);
                value_ = make_angle(value_ / n);
                assert(invariant());
                return *this;
        }

        // observers

        /* implicit */ constexpr operator int() const noexcept
        {
                return value_;
        }

        // operators

        friend constexpr auto operator+(Angle const& a) noexcept
        {
                return Angle{+a.value_};
        }

        friend constexpr auto operator-(Angle const& a) noexcept
        {
                return Angle{-a.value_};
        }

private:
        // implementation

        constexpr int make_angle(int n) const noexcept
        {
                return detail::abs_remainder(n, 360);
        }

        // contracts

        constexpr bool invariant() const noexcept
        {
                return 0 <= value_ && value_ < 360;
        }

        // representation

        int value_{};
};

inline
constexpr auto operator+(Angle const& lhs, Angle const& rhs) noexcept
{
        auto nrv(lhs);
        nrv += rhs;
        return nrv;
}

inline
constexpr auto operator-(Angle const& lhs, Angle const& rhs) noexcept
{
        auto nrv(lhs);
        nrv -= rhs;
        return nrv;
}

inline
constexpr auto operator*(Angle const& a, int n) noexcept
{
        auto nrv(a);
        nrv *= n;
        return nrv;
}

inline
constexpr auto operator*(int n, Angle const& a) noexcept
{
        auto nrv(a);
        nrv *= n;
        return nrv;
}

inline
constexpr auto operator/(Angle const& a, int n)
{
        assert(n != 0);
        auto nrv(a);
        nrv /= n;
        return nrv;
}

}       // namespace dctl
