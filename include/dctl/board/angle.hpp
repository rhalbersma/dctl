#pragma once
#include <xstd/cstdlib.hpp>             // euclidean_div
#include <experimental/type_traits>
#include <cassert>                      // assert
#include <type_traits>                  // integral_constant

namespace dctl {

class angle
{
        int const degrees_ = 0;

        constexpr auto assert_invariant() const noexcept
        {
                assert(0 <= degrees_ && degrees_ < 360);
        }

        static constexpr auto assert_type_traits() noexcept
        {
                using T = angle;
                static_assert( std::experimental::is_trivially_destructible_v<T>);
                static_assert(!std::experimental::is_trivially_default_constructible_v<T>);
                static_assert( std::experimental::is_nothrow_default_constructible_v<T>);
                static_assert( std::experimental::is_trivially_copy_constructible_v<T>);
                static_assert( std::experimental::is_trivially_move_constructible_v<T>);
                static_assert(!std::experimental::is_copy_assignable_v<T>);
                static_assert(!std::experimental::is_move_assignable_v<T>);
                static_assert( std::experimental::is_standard_layout_v<T>);
                static_assert( std::experimental::is_literal_type_v<T>);
        }

public:
        angle() = default;

        template<class Integral>
        explicit constexpr angle(Integral const n) noexcept
        :
                degrees_{xstd::euclidean_div(static_cast<int>(n), 360).rem}
        {
                assert_invariant();
        }

        constexpr auto degrees() const noexcept
        {
                return degrees_;
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
        return a.degrees() == b.degrees();
}

constexpr auto operator!=(angle const a, angle const b) noexcept
{
        return !(a == b);
}

constexpr auto operator+(angle const a) noexcept
{
        return angle{+a.degrees()};
}

constexpr auto operator-(angle const a) noexcept
{
        return angle{-a.degrees()};
}

constexpr auto operator+(angle const a, angle const b) noexcept
{
        return angle{a.degrees() + b.degrees()};
}

constexpr auto operator-(angle const a, angle const b) noexcept
{
        return angle{a.degrees() - b.degrees()};
}

template<class Integral>
constexpr auto operator*(angle const a, Integral const n) noexcept
{
        return angle{a.degrees() * static_cast<int>(n)};
}

template<class Integral>
constexpr auto operator*(Integral const n, angle const a) noexcept
{
        return angle{static_cast<int>(n) * a.degrees()};
}

constexpr auto inverse(angle const a) noexcept
{
        return -a;
}

constexpr auto rotate(angle const a, angle const b) noexcept
{
        return a + b;
}

constexpr auto mirror(angle const a, angle const b) noexcept
{
        // a.rotate(inverse(b)).inverse().rotate(b)
        return rotate(inverse(rotate(a, inverse(b))), b);
}

/*

                up == 90
                   |
   135 == left_up  |  right_up == 45
                 \ | /
                  \|/
  180 == left ----- ----- right == 0
                  /|\
                 / | \
 225 == left_down  |  right_down == 315
                   |
              270 == down

*/

template<int N>
using angle_constant = std::integral_constant<int, angle{N}.degrees()>;

template<int N> using right      = angle_constant<N +   0>;
template<int N> using right_up   = angle_constant<N +  45>;
template<int N> using up         = angle_constant<N +  90>;
template<int N> using left_up    = angle_constant<N + 135>;
template<int N> using left       = angle_constant<N + 180>;
template<int N> using left_down  = angle_constant<N + 225>;
template<int N> using down       = angle_constant<N + 270>;
template<int N> using right_down = angle_constant<N + 315>;

constexpr auto is_orthogonal(angle const a) noexcept
{
        return a.degrees() % 90 == 0;
}

constexpr auto is_diagonal(angle const a) noexcept
{
        return a.degrees() % 90 == 45;
}

constexpr auto is_up(angle const a) noexcept
{
        return 0 < a.degrees() && a.degrees() < 180;
}

constexpr auto is_down(angle const a) noexcept
{
        return 180 < a.degrees();
}

constexpr auto is_left(angle const a) noexcept
{
        return 90 < a.degrees() && a.degrees() < 270;
}

constexpr auto is_right(angle const a) noexcept
{
        return 270 < a.degrees() || a.degrees() < 90;
}

constexpr auto is_positive(angle const a) noexcept
{
        return 0 < a.degrees() && a.degrees() <= 180;
}

constexpr auto is_negative(angle const a) noexcept
{
        return a.degrees() == 0 || 180 < a.degrees();
}

}       // namespace dctl
