#pragma once
#include <dctl/board/angle.hpp>                       // Angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/mask/squares.hpp>          // Squares
#include <dctl/board/ray/fill.hpp>                    // fill
#include <dctl/board/ray/iterator.hpp>                // Iterator
#include <dctl/board/set_type.hpp>                    // set_type
#include <dctl/utility/make_array.hpp>          // make_array
#include <array>                                // array
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class KingTargets
{
        template<int Direction>
        static constexpr auto init(std::size_t sq) noexcept
        {
                constexpr auto squares = squares_v<Board>;
                return squares.test(sq) ? ray::fill(ray::make_iterator<Board, Direction>(sq), squares) : get_set_type<Board>{};
        }

        static constexpr auto theta = 45_deg;
        static constexpr auto beta  =  0_deg;

        using table_type = std::array<get_set_type<Board>, Board::bits()>;
        static table_type const table[];

public:
        static constexpr auto mask(Angle alpha, std::size_t sq) noexcept
        {
                auto const segment = (alpha - beta) / theta;
                return table[segment][sq];
        }
};

template<class Board>
constexpr Angle
KingTargets<Board>::theta;

template<class Board>
constexpr Angle
KingTargets<Board>::beta;

template<class Board>
typename KingTargets<Board>::table_type const
KingTargets<Board>::table[] =
{
        make_array<Board::bits()>(init<  0_deg>),
        make_array<Board::bits()>(init< 45_deg>),
        make_array<Board::bits()>(init< 90_deg>),
        make_array<Board::bits()>(init<135_deg>),
        make_array<Board::bits()>(init<180_deg>),
        make_array<Board::bits()>(init<225_deg>),
        make_array<Board::bits()>(init<270_deg>),
        make_array<Board::bits()>(init<315_deg>)
};

}       // namespace board
}       // namespace dctl
