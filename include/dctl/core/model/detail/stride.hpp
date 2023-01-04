#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>
#include <dctl/util/type_traits.hpp>
#include <xstd/cstdlib.hpp>             // abs
#include <cassert>
#include <concepts>                     // convertible_to

namespace dctl::core {

template<class Board>
class stride
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
        constexpr auto operator()(angle a) const
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

template<class Board, class Direction>
inline constexpr auto stride_v = stride<Board>{}(angle{Direction::value});

template<class Direction>
inline constexpr auto is_left_shift_v = angle{Direction::value} == 0_deg || 180_deg < angle{Direction::value};

template<class Direction>
[[nodiscard]] constexpr auto find_first(auto const& set) noexcept
{
        if constexpr (is_left_shift_v<Direction>) {
                return set.front();
        } else {
                return set.back();
        }
}

template<class Board, class Direction, int Distance = 1>
struct advance
{
        template<class Set>
        constexpr auto operator()(Set& bs) const noexcept
        {
                constexpr auto n = xstd::abs(Distance) * stride_v<Board, Direction>;
                static_assert(0 <= n && n < Set::max_size());
                if constexpr (!(is_left_shift_v<Direction> ^ (Distance >= 0))) {
                        bs <<= n;
                } else {
                        bs >>= n;
                }
        }

        constexpr auto operator()(int& sq) const noexcept
        {
                constexpr auto n = Distance * stride_v<Board, Direction>;
                if constexpr (is_left_shift_v<Direction>) {
                        sq += n;
                } else {
                        sq -= n;
                }
        }
};

template<class Board, class Direction, int Distance = 1>
struct next
{
        template<class Arg>
                requires (!std::convertible_to<Arg, int>)
        [[nodiscard]] constexpr auto operator()(Arg arg) const noexcept
        {
                advance<Board, Direction, Distance>{}(arg);
                return arg;
        }

        [[nodiscard]] constexpr auto operator()(int arg) const noexcept
        {
                advance<Board, Direction, Distance>{}(arg);
                return arg;
        }
};

template<class Board, class Direction, int Distance = 1>
struct prev
{
        template<class Arg>
                requires (!std::convertible_to<Arg, int>)
        [[nodiscard]] constexpr auto operator()(Arg arg) const noexcept
        {
                advance<Board, Direction, -Distance>{}(arg);
                return arg;
        }

        [[nodiscard]] constexpr auto operator()(int arg) const noexcept
        {
                advance<Board, Direction, -Distance>{}(arg);
                return arg;
        }
};

}       // namespace dctl::core
