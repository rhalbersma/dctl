#pragma once
#include <dctl/board/angle.hpp>         // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/ray/fill.hpp>      // fill
#include <dctl/board/ray/iterator.hpp>  // iterator
#include <dctl/mask/squares.hpp>        // squares
#include <dctl/utility/fill_array.hpp>  // fill_array
#include <dctl/utility/type_traits.hpp> // set_t
#include <array>                        // array
#include <cstddef>                      // size_t

namespace dctl {
namespace mask {

template<class Board>
class king_targets
{
        template<int Direction>
        struct init
        {
                auto operator()(std::size_t const sq) const noexcept
                {
                        constexpr auto squares = squares_v<Board>;
                        return squares.test(sq) ? board::ray::fill(board::ray::make_iterator<Board, Direction>(sq), squares) : set_t<Board>{};
                }
        };

        static constexpr auto theta = 45_deg;
        static constexpr auto beta  =  0_deg;

        using value_type = std::array<set_t<Board>, Board::bits()>;

        static value_type const value[];

public:
        auto operator()(std::size_t const sq, angle const alpha) const noexcept
        {
                auto const segment = (alpha.degrees - beta.degrees) / theta.degrees;
                return value[segment][sq];
        }
};

template<class Board>
constexpr angle
king_targets<Board>::theta;

template<class Board>
constexpr angle
king_targets<Board>::beta;

template<class Board>
typename king_targets<Board>::value_type const
king_targets<Board>::value[] =
{
        fill_array<Board::bits()>(init<  0>{}),
        fill_array<Board::bits()>(init< 45>{}),
        fill_array<Board::bits()>(init< 90>{}),
        fill_array<Board::bits()>(init<135>{}),
        fill_array<Board::bits()>(init<180>{}),
        fill_array<Board::bits()>(init<225>{}),
        fill_array<Board::bits()>(init<270>{}),
        fill_array<Board::bits()>(init<315>{})
};

}       // namespace mask
}       // namespace dctl
