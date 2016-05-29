#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/color.hpp>                       // black, white
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Initial
{
        template<Color ToMove>
        struct init
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_initial
                {
                        Color const to_move;
                        std::size_t const rows_;

                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                auto const separation = Board::height - 2 * rows_;
                                auto const y = detail::to_llo(sq, Board::inner_grid).y;
                                auto const min_y = to_move == Color::white ? 0 : Board::height - (Board::height - separation) / 2;
                                auto const max_y = to_move == Color::white ? (Board::height - separation) / 2 : Board::height;
                                return min_y <= y && y < max_y;
                        }
                };

                constexpr auto operator()(std::size_t const rows) const noexcept
                {
                        return make_set_if<Board>(is_initial{ToMove, rows});
                }
        };

        static constexpr auto N = Board::height / 2 + 1;
        using table_type = std::array<set_t<Board>, N>;

        static constexpr table_type table[] =
        {
                fill_array<N>(init<Color::black>{}),
                fill_array<N>(init<Color::white>{})
        };

public:
        static constexpr auto mask(Color const c, std::size_t const separation) noexcept
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation && separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(0 <= rows && rows <= Board::height / 2);
                return table[xstd::to_underlying_type(c)][rows];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type
Initial<Board>::table[];

}       // namespace board
}       // namespace dctl
