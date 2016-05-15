#pragma once
#include <dctl/board/angle.hpp>         // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/mask/squares.hpp>  // Squares
#include <dctl/board/ray/fill.hpp>      // fill
#include <dctl/board/ray/iterator.hpp>  // Iterator
#include <dctl/utility/make_array.hpp>  // make_array
#include <dctl/utility/type_traits.hpp> // set_t
#include <array>                        // array
#include <cstddef>                      // size_t

namespace dctl {
namespace board {

template<class Board>
class KingTargets
{
        template<int Direction>
        struct init
        {
                constexpr auto operator()(std::size_t sq) const noexcept
                {
                        constexpr auto squares = squares_v<Board>;
                        return squares.test(sq) ? ray::fill(ray::make_iterator<Board, Direction>(sq), squares) : set_t<Board>{};
                }
        };

        static constexpr auto theta = 45_deg;
        static constexpr auto beta  =  0_deg;

        using table_type = std::array<set_t<Board>, Board::bits()>;
        static table_type const table[];

public:
        static constexpr auto mask(angle const alpha, std::size_t const sq) noexcept
        {
                auto const segment = (alpha.degrees() - beta.degrees()) / theta.degrees();
                return table[segment][sq];
        }
};

template<class Board>
constexpr angle
KingTargets<Board>::theta;

template<class Board>
constexpr angle
KingTargets<Board>::beta;

template<class Board>
typename KingTargets<Board>::table_type const
KingTargets<Board>::table[] =
{
        make_array<Board::bits()>(init<  0>{}),
        make_array<Board::bits()>(init< 45>{}),
        make_array<Board::bits()>(init< 90>{}),
        make_array<Board::bits()>(init<135>{}),
        make_array<Board::bits()>(init<180>{}),
        make_array<Board::bits()>(init<225>{}),
        make_array<Board::bits()>(init<270>{}),
        make_array<Board::bits()>(init<315>{})
};

}       // namespace board
}       // namespace dctl
