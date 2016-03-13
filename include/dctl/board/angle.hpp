#pragma once
#include <xstd/cstdlib.hpp>     // euclidean_div
#include <cassert>              // assert
#include <type_traits>          // integral_constant

namespace dctl {

// angle has the algebraic structure of a bimodule over the ring of integers,
// i.e. an abelian group w.r.t. addition, closed under integer multiplication:
// for angles A and B and integers n and m, n * A + m * B is also an angle.

class angle
{
        int const degrees_ = 0;

        static constexpr auto as_angle(int const n) noexcept
        {
                return xstd::euclidean_div(n, 360).rem;
        }

        static constexpr auto assert_traits() noexcept
        {
                using T = angle;
                static_assert( std::is_nothrow_default_constructible_v<T>);
                static_assert( std::is_trivially_copy_constructible_v<T>);
                static_assert( std::is_trivially_move_constructible_v<T>);
                static_assert(!std::is_copy_assignable_v<T>);
                static_assert(!std::is_move_assignable_v<T>);
                static_assert( std::is_trivially_copyable_v<T>);
                static_assert( std::is_standard_layout_v<T>);
                static_assert( std::is_literal_type_v<T>);
        }

        constexpr auto invariant() const noexcept
        {
                return 0 <= degrees_ && degrees_ < 360;
        }
public:
        angle() = default;

        explicit constexpr angle(int const n) noexcept
        :
                degrees_{as_angle(n)}
        {
                assert(invariant());
        }

        constexpr auto degrees() const noexcept
        {
                return degrees_;
        }
};

inline namespace literals {
inline namespace angle_literals {

inline
constexpr auto operator"" _deg(unsigned long long const n) noexcept
{
        return angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals

inline
constexpr auto operator==(angle const a, angle const b) noexcept
{
        return a.degrees() == b.degrees();
}

inline
constexpr auto operator!=(angle const a, angle const b) noexcept
{
        return !(a == b);
}

inline
constexpr auto operator+(angle const a) noexcept
{
        return angle{+a.degrees()};
}

inline
constexpr auto operator-(angle const a) noexcept
{
        return angle{-a.degrees()};
}

inline
constexpr auto operator+(angle const a, angle const b) noexcept
{
        return angle{a.degrees() + b.degrees()};
}

inline
constexpr auto operator-(angle const a, angle const b) noexcept
{
        return angle{a.degrees() - b.degrees()};
}

inline
constexpr auto operator*(angle const a, int const n) noexcept
{
        return angle{a.degrees() * n};
}

inline
constexpr auto operator*(int const n, angle const a) noexcept
{
        return angle{n * a.degrees()};
}

inline
constexpr auto inverse(angle const a) noexcept
{
        return -a;
}

inline
constexpr auto rotate(angle const a, angle const b) noexcept
{
        return a + b;
}

inline
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

inline
constexpr auto is_orthogonal(angle const a) noexcept
{
        return a.degrees() % 90 == 0;
}

inline
constexpr auto is_diagonal(angle const a) noexcept
{
        return a.degrees() % 90 == 45;
}

inline
constexpr auto is_up(angle const a) noexcept
{
        return 0 < a.degrees() && a.degrees() < 180;
}

inline
constexpr auto is_down(angle const a) noexcept
{
        return 180 < a.degrees();
}

inline
constexpr auto is_left(angle const a) noexcept
{
        return 90 < a.degrees() && a.degrees() < 270;
}

inline
constexpr auto is_right(angle const a) noexcept
{
        return 270 < a.degrees() || a.degrees() < 90;
}

inline
constexpr auto is_positive(angle const a) noexcept
{
        return 0 < a.degrees() && a.degrees() <= 180;
}

inline
constexpr auto is_negative(angle const a) noexcept
{
        return a.degrees() == 0 || 180 < a.degrees();
}

}       // namespace dctl
