#pragma once
#include <cassert>                      // assert
#include <iosfwd>                       // ostream
#include <dctl/utility/abs.hpp>         // abs_remainder
#include <dctl/utility/range.hpp>       // is_element

namespace dctl {

class Angle
{
public:
        // structors

        constexpr Angle() noexcept = default;

        explicit constexpr Angle(int n) noexcept
        :
                value_{make_angle(n)}
        {
                assert(invariant());
        }

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

        // operators

        friend constexpr auto operator+(Angle const& a) noexcept
        {
                return Angle{+a.value_};
        }

        friend constexpr auto operator-(Angle const& a) noexcept
        {
                return Angle{-a.value_};
        }

        friend constexpr auto operator+(Angle const& L, Angle const& R) noexcept
        {
                Angle nrv{L};
                nrv += R;
                return nrv;
        }

        friend constexpr auto operator-(Angle const& L, Angle const& R) noexcept
        {
                Angle nrv{L};
                nrv -= R;
                return nrv;
        }

        // predicates

        friend constexpr auto operator==(Angle const& L, Angle const& R) noexcept
        {
                return L.value_ == R.value_;
        }

        friend constexpr auto operator!=(Angle const& L, Angle const& R) noexcept
        {
                return !(L == R);
        }

        friend constexpr auto operator<(Angle const& L, Angle const& R) noexcept
        {
                return L.value_ < R.value_;
        }


        // views

        constexpr operator int() const noexcept
        {
                return value_;
        }

        friend auto& operator<<(std::ostream& os, Angle const& a)
        {
                return os << a.value_;
        }

private:
        // implementation

        static constexpr int make_angle(int n) noexcept
        {
                return util::abs_remainder(n, 360);
        }

        constexpr bool invariant() const noexcept
        {
                return util::is_element(value_, {0, 360});
        }

        // representation

        int value_{0};
};

}       // namespace dctl
