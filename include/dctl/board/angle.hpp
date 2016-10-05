#pragma once
#include <xstd/cstdlib.hpp>     // euclidean_div
#include <cassert>              // assert
#include <type_traits>          // integral_constant

namespace dctl {
namespace board {

class Angle
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = Angle;

                static_assert(std::is_trivially_destructible<T>{});
                static_assert(std::is_trivially_default_constructible<T>{});
                static_assert(std::is_nothrow_constructible<T, int>{});
                static_assert(std::is_trivially_copy_constructible<T>{});
                static_assert(std::is_trivially_copy_assignable<T>{});
                static_assert(std::is_trivially_move_constructible<T>{});
                static_assert(std::is_trivially_move_assignable<T>{});

                static_assert(std::is_pod<T>{});
        }

        constexpr auto assert_invariants() const noexcept
        {
                assert(0 <= degrees()); assert(degrees() < 360);
        }

        int degrees_;
public:
        Angle() = default;

        explicit constexpr Angle(int const n) noexcept
        :
                degrees_{xstd::euclidean_div(n, 360).rem}
        {
                assert_invariants();
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
        return Angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals

constexpr auto operator==(Angle const a, Angle const b) noexcept
{
        return a.degrees() == b.degrees();
}

constexpr auto operator!=(Angle const a, Angle const b) noexcept
{
        return !(a == b);
}

constexpr auto operator+(Angle const a) noexcept
{
        return Angle{+a.degrees()};
}

constexpr auto operator-(Angle const a) noexcept
{
        return Angle{-a.degrees()};
}

constexpr auto operator+(Angle const a, Angle const b) noexcept
{
        return Angle{a.degrees() + b.degrees()};
}

constexpr auto operator-(Angle const a, Angle const b) noexcept
{
        return Angle{a.degrees() - b.degrees()};
}

constexpr auto operator*(Angle const a, int const n) noexcept
{
        return Angle{a.degrees() * n};
}

constexpr auto operator*(int const n, Angle const a) noexcept
{
        return Angle{n * a.degrees()};
}

constexpr auto inverse(Angle const a) noexcept
{
        return -a;
}

constexpr auto rotate(Angle const a, Angle const b) noexcept
{
        return a + b;
}

constexpr auto reverse(Angle const a) noexcept
{
        return rotate(a, 180_deg);
}

constexpr auto mirror(Angle const a, Angle const b) noexcept
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
using Angle_constant = std::integral_constant<int, Angle{N}.degrees()>;

template<int N> using right      = Angle_constant<N +   0>;
template<int N> using right_up   = Angle_constant<N +  45>;
template<int N> using up         = Angle_constant<N +  90>;
template<int N> using left_up    = Angle_constant<N + 135>;
template<int N> using left       = Angle_constant<N + 180>;
template<int N> using left_down  = Angle_constant<N + 225>;
template<int N> using down       = Angle_constant<N + 270>;
template<int N> using right_down = Angle_constant<N + 315>;

constexpr auto is_orthogonal(Angle const a) noexcept
{
        return a.degrees() % 90 == 0;
}

constexpr auto is_diagonal(Angle const a) noexcept
{
        return a.degrees() % 90 == 45;
}

constexpr auto is_up(Angle const a) noexcept
{
        return 0 < a.degrees() && a.degrees() < 180;
}

constexpr auto is_down(Angle const a) noexcept
{
        return 180 < a.degrees();
}

constexpr auto is_left(Angle const a) noexcept
{
        return 90 < a.degrees() && a.degrees() < 270;
}

constexpr auto is_right(Angle const a) noexcept
{
        return 270 < a.degrees() || a.degrees() < 90;
}

}       // namespace board
}       // namespace dctl
