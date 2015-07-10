#pragma once
#include <dctl/board/coordinates.hpp>           // to_llo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/player.hpp>                       // black, white
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/utility/make_array.hpp>          // make_array
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Column
{
        template<Player ToMove>
        static constexpr auto init(std::size_t column) noexcept
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_column
                {
                        Player const to_move;
                        std::size_t const column_;

                        constexpr auto operator()(std::size_t sq) const noexcept
                        {
                                assert(column_ < Board::width());
                                return to_llo(sq, Board::inner_grid).x() == (to_move == Player::white ? column_ : Board::width() - 1 - column_);
                        }
                };

                return make_set_if<Board>(is_column{ToMove, column});
        }

        using table_type = std::array<get_set_type<Board>, Board::width()>;
        static constexpr table_type table[] =
        {
                make_array<Board::width()>(init<Player::black>),
                make_array<Board::width()>(init<Player::white>)
        };

public:
        static constexpr auto mask(Player c, std::size_t column) noexcept
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
