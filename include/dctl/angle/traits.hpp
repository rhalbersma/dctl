#pragma once

namespace dctl {
namespace angle {

/*

        The standard enumeration of angles

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

template<class T>
constexpr auto is_orthogonal(T const& alpha) noexcept
{
        return alpha % T{90} == T{0};
}

template<class T>
constexpr auto is_diagonal(T const& alpha) noexcept
{
        return alpha % T{90} == T{45};
}

template<class T>
constexpr auto is_up(T const& alpha) noexcept
{
        return T{0} < alpha && alpha < T{180};
}

template<class T>
constexpr auto is_down(T const& alpha) noexcept
{
        return T{180} < alpha;
}

template<class T>
constexpr auto is_left(T const& alpha) noexcept
{
        return T{90} < alpha && alpha < T{270};
}

template<class T>
constexpr auto is_right(T const& alpha) noexcept
{
        return T{270} < alpha || (T{0} <= alpha && alpha < T{90});
}

template<class T>
constexpr auto is_positive(T const& alpha) noexcept
{
        return T{0} < alpha && alpha <= T{180};
}

template<class T>
constexpr auto is_negative(T const& alpha) noexcept
{
        return alpha == T{0} || T{180} < alpha;
}

}       // namespace angle
}       // namespace dctl
