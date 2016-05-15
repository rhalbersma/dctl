#pragma once
#include <dctl/board/coordinates.hpp>           // to_llo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/color.hpp>                       // black, white
#include <dctl/utility/make_array.hpp>          // make_array
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
        template<Color ToMove>
        struct init
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_column
                {
                        Color const to_move;
                        int const column_;

                        constexpr auto operator()(int sq) const noexcept
                        {
                                assert(column_ < Board::width());
                                return to_llo(sq, Board::inner_grid).x() == (to_move == Color::white ? column_ : Board::width() - 1 - column_);
                        }
                };

                constexpr auto operator()(int column) const noexcept
                {
                        return make_set_if<Board>(is_column{ToMove, column});
                }
        };

        using table_type = std::array<set_t<Board>, Board::width()>;
        static constexpr table_type table[] =
        {
                make_array<Board::width()>(init<Color::black>{}),
                make_array<Board::width()>(init<Color::white>{})
        };

public:
        static constexpr auto mask(Color c, std::size_t column) noexcept
        {
                assert(column < Board::width());
                return table[xstd::to_underlying_type(c)][column];
        }
};

template<class Board>
constexpr typename Column<Board>::table_type
Column<Board>::table[];

}       // namespace board
}       // namespace dctl
