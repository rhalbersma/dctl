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
constexpr bool is_orthogonal(T const& t)
{
        return t % 90 == 0;
}

template<class T>
constexpr bool is_diagonal(T const& t)
{
        return t % 90 == 45;
}

template<class T>
constexpr bool is_up(T const& t)
{
        return 0 < t && t < 180;
}

template<class T>
constexpr bool is_down(T const& t)
{
        return 180 < t;
}

template<class T>
constexpr bool is_left(T const& t)
{
        return 90 < t && t < 270;
}

template<class T>
constexpr bool is_right(T const& t)
{
        return 270 < t || (0 <= t && t < 90);
}

template<class T>
constexpr bool is_positive(T const& t)
{
        return 0 < t && t <= 180;
}

template<class T>
constexpr bool is_negative(T const& t)
{
        return t == 0 || 180 < t;
}

}       // namespace angle
}       // namespace dctl
