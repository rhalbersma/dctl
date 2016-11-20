#pragma once
#include <dctl/board/angle.hpp>
#include <cassert>

namespace dctl {
namespace board {

enum class direction
{
        left,
        right
};

template<direction>
struct first;

template<>
struct first<direction::left>
{
        template<class Set>
        constexpr auto operator()(Set const s) const
        {
                return *s.cbegin();
        }
};

template<>
struct first<direction::right>
{
        template<class Set>
        constexpr auto operator()(Set const s) const
        {
                return *s.crbegin();
        }
};

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

struct shift_sign
{
        constexpr auto operator()(angle const a) const noexcept
        {
                return (a == 0_deg || 180_deg < a) ? direction::left : direction::right;
        }
};

template<int Direction>
constexpr auto shift_sign_v = shift_sign{}(angle{Direction});

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
                case   0 : return right()     ;
                case  45 : return right_up()  ;
                case  90 : return up()        ;
                case 135 : return left_up()   ;
                case 180 : return left()      ;
                case 225 : return left_down() ;
                case 270 : return down()      ;
                case 315 : return right_down();
                default  : assert(false); return 0;
                }
        }
};

template<class Board, int Direction>
constexpr auto shift_size_v = shift_size<Board>{}(angle{Direction});

}       // namespace board
}       // namespace dctl
