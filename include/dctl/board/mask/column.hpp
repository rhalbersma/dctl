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
class Column
{
        template<color ToMove>
        struct init
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_column
                {
                        color const to_move;
                        std::size_t const column_;

                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                assert(column_ < Board::width);
                                return detail::to_llo(sq, Board::inner_grid).x == (to_move == color::white ? column_ : Board::width - 1 - column_);
                        }
                };

                constexpr auto operator()(std::size_t const column) const noexcept
                {
                        return make_set_if<Board>(is_column{ToMove, column});
                }
        };

        using table_type = std::array<set_t<Board>, Board::width>;
        static constexpr table_type table[] =
        {
                fill_array<Board::width>(init<color::black>{}),
                fill_array<Board::width>(init<color::white>{})
        };

public:
        static constexpr auto mask(color const c, std::size_t const column) noexcept
        {
                assert(column < Board::width);
                return table[xstd::to_underlying_type(c)][column];
        }
};

template<class Board>
constexpr typename Column<Board>::table_type
Column<Board>::table[];

}       // namespace board
}       // namespace dctl
