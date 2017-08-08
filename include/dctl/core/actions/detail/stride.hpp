#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>
#include <cassert>

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
constexpr auto stride_v = stride<Board>{}(angle{Direction});

template<int Direction>
constexpr auto is_forward_v = angle{Direction} == 0_deg || 180_deg < angle{Direction};

template<int Direction, class Set>
auto find_first(Set const& s)
{
        if constexpr (is_forward_v<Direction>) {
                return s.front();
        } else {
                return s.back();
        }
}

template<class Board, int Direction, int Distance = 1>
struct advance
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s <<= n;
                } else {
                        return s >>= n;
                }
        }

        constexpr auto operator()(int& sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq += n;
                } else {
                        return sq -= n;
                }
        }
};

template<class Board, int Direction, int Distance = 1>
struct next
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type const& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s << n;
                } else {
                        return s >> n;
                }
        }

        constexpr auto operator()(int const sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq + n;
                } else {
                        return sq - n;
                }
        }
};

template<class Board, int Direction, int Distance = 1>
struct prev
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type const& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s >> n;
                } else {
                        return s << n;
                }
        }

        constexpr auto operator()(int const sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq - n;
                } else {
                        return sq + n;
                }
        }
};

}       // namespace dctl::core
