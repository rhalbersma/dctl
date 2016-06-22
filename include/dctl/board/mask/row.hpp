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
class Row
{
        template<color ToMove>
        struct init
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_row
                {
                        color const to_move;
                        std::size_t const row_;

                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                assert(row_ < Board::height);
                                return detail::to_llo(sq, Board::inner_grid).y == (to_move == color::white ? row_ : Board::height - 1 - row_);
                        }
                };

                constexpr auto operator()(std::size_t const row) const noexcept
                {
                        return make_set_if<Board>(is_row{ToMove, row});
                }
        };

        using table_type = std::array<set_t<Board>, Board::height>;
        static constexpr table_type table[] =
        {
                fill_array<Board::height>(init<color::black>{}),
                fill_array<Board::height>(init<color::white>{})
        };

public:
        static constexpr auto mask(color const c, std::size_t const row) noexcept
        {
                assert(row < Board::height);
                return table[xstd::to_underlying_type(c)][row];
        }
};

template<class Board>
constexpr typename Row<Board>::table_type
Row<Board>::table[];

}       // namespace board
}       // namespace dctl
