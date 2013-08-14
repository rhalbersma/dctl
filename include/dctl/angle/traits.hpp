#pragma once

namespace dctl {
namespace angle {

/*

                D090 = L090
                     |
        D135 = L135  |  D045 = L045
                   \ | /
                    \|/
           D180 ----- ----- D000 = D360
                    /|\
                   / | \
        D225 = R135  |  D315 = R045
                     |
                D270 = R090

*/

template<class T>
constexpr auto is_orthogonal(T const& t) noexcept
{
        return t % 90 == 0;
}

template<class T>
constexpr auto is_diagonal(T const& t) noexcept
{
        return t % 90 == 45;
}

template<class T>
constexpr auto is_up(T const& t) noexcept
{
        return 0 < t && t < 180;
}

template<class T>
constexpr auto is_down(T const& t) noexcept
{
        return 180 < t;
}

template<class T>
constexpr auto is_left(T const& t) noexcept
{
        return 90 < t && t < 270;
}

template<class T>
constexpr auto is_right(T const& t) noexcept
{
        return 270 < t || (0 <= t && t < 90);
}

template<class T>
constexpr auto is_positive(T const& t) noexcept
{
        return 0 < t && t <= 180;
}

template<class T>
constexpr auto is_negative(T const& t) noexcept
{
        return t == 0 || 180 < t;
}

}       // namespace angle
}       // namespace dctl
