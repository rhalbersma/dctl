#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>
#include <cassert>

namespace dctl::core {

enum class direction
{
        left,
        right
};

template<int Direction>
constexpr auto shift_sign_v = (angle{Direction} == 0_deg || 180_deg < angle{Direction}) ? direction::left : direction::right;

template<direction>
struct first;

template<>
struct first<direction::left>
{
        template<class Set>
        constexpr auto operator()(Set const s) const
        {
                return s.front();
        }
};

template<>
struct first<direction::right>
{
        template<class Set>
        constexpr auto operator()(Set const s) const
        {
                return s.back();
        }
};

template<int Direction, class Set>
auto find_first(Set const& s)
{
        return first<shift_sign_v<Direction>>{}(s);
}

template<direction, int N>
struct shift;

template<int N>
struct shift<direction::left, N>
{
        template<class T>
        constexpr auto operator()(T const& t) const noexcept
        {
                return t << N;
        }
};

template<int N>
struct shift<direction::right, N>
{
        template<class T>
        constexpr auto operator()(T const& t) const noexcept
        {
                return t >> N;
        }
};

template<direction, int N>
struct shift_assign;

template<int N>
struct shift_assign<direction::left, N>
{
        template<class T>
        constexpr auto operator()(T& t) const noexcept
        {
                t <<= N;
        }
};

template<int N>
struct shift_assign<direction::right, N>
{
        template<class T>
        constexpr auto operator()(T& t) const noexcept
        {
                t >>= N;
        }
};

template<class Board>
class shift_size
{
        constexpr auto left_down () const noexcept { return (Board::outer_grid.width() + Board::outer_grid.edge()) / 2; }
        constexpr auto right_down() const noexcept { return left_down() + 1;            }
        constexpr auto right     () const noexcept { return right_down() - left_down(); }
        constexpr auto down      () const noexcept { return right_down() + left_down(); }
        constexpr auto left_up   () const noexcept { return right_down();               }
        constexpr auto right_up  () const noexcept { return left_down();                }
        constexpr auto left      () const noexcept { return right();                    }
        constexpr auto up        () const noexcept { return down();                     }
public:
        constexpr auto operator()(angle const a) const
        {
                switch (a.value()) {
                case   0: return right()     ;
                case  45: return right_up()  ;
                case  90: return up()        ;
                case 135: return left_up()   ;
                case 180: return left()      ;
                case 225: return left_down() ;
                case 270: return down()      ;
                case 315: return right_down();
                default: assert(false); return 0;
                }
        }
};

template<class Board, int Direction>
constexpr auto shift_size_v = shift_size<Board>{}(angle{Direction});

}       // namespace dctl::core
