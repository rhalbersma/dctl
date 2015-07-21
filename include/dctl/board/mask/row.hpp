#pragma once
#include <dctl/board/coordinates.hpp>           // to_llo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/color.hpp>                       // black, white
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/utility/make_array.hpp>          // make_array
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Row
{
        template<Color ToMove>
        static constexpr auto init(std::size_t row) noexcept
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_row
                {
                        Color const to_move;
                        std::size_t const row_;

                        constexpr auto operator()(std::size_t sq) const noexcept
                        {
                                assert(row_ < Board::height());
                                return to_llo(sq, Board::inner_grid).y() == (to_move == Color::white ? row_ : Board::height() - 1 - row_);
                        }
                };

                return make_set_if<Board>(is_row{ToMove, row});
        }

        using table_type = std::array<get_set_type<Board>, Board::height()>;
        static constexpr table_type table[] =
        {
                make_array<Board::height()>(init<Color::black>),
                make_array<Board::height()>(init<Color::white>)
        };

public:
        static constexpr auto mask(Color c, std::size_t row) noexcept
        {
                assert(row < Board::height());
                return table[xstd::to_underlying_type(c)][row];
        }
};

template<class Board>
constexpr typename Row<Board>::table_type
Row<Board>::table[];

}       // namespace board
}       // namespace dctl
