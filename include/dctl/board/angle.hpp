#pragma once
#include <dctl/utility/concepts.hpp>    // is_trivial_special_members
#include <xstd/cstdlib.hpp>             // euclidean_div
#include <cassert>                      // assert
#include <type_traits>                  // integral_constant, is_pod

namespace dctl {
namespace board {

class angle
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = angle;
                static_assert(util::is_trivial_special_members_v<T>);
                static_assert(std::is_pod<T>{});
                static_assert(std::is_nothrow_constructible<T, int>{});
        }

        constexpr auto assert_invariants() const noexcept
        {
                assert(0 <= m_value); assert(m_value < 360);
        }

        int m_value;
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

constexpr auto operator<(angle const a, angle const b) noexcept
{
        return a.value() < b.value();
}

constexpr auto operator!=(angle const a, angle const b) noexcept
{
        return !(a == b);
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

constexpr auto operator/(angle const a, int const n) noexcept // Throws: Nothing.
{
        assert(n != 0);
        return angle{a.value() / n};
}

constexpr auto operator/(angle const a, angle const b) noexcept // Throws: Nothing.
{
        assert(b.value() != 0);
        return a.value() / b.value();
}

constexpr auto operator%(angle const a, angle const b) noexcept // Throws: Nothing.
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

namespace detail {

template<int N> using angle_holder = std::integral_constant<int, angle{N}.value()>;

}       // namespace detail

template<int N> using right      = detail::angle_holder<N +   0>;
template<int N> using right_up   = detail::angle_holder<N +  45>;
template<int N> using up         = detail::angle_holder<N +  90>;
template<int N> using left_up    = detail::angle_holder<N + 135>;
template<int N> using left       = detail::angle_holder<N + 180>;
template<int N> using left_down  = detail::angle_holder<N + 225>;
template<int N> using down       = detail::angle_holder<N + 270>;
template<int N> using right_down = detail::angle_holder<N + 315>;

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

}       // namespace board
}       // namespace dctl
