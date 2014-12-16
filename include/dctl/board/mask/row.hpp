#pragma once
#include <dctl/board/coordinates.hpp>           // ulo_from_sq
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/color.hpp>                       // black, white
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/utility/make_array.hpp>          // make_array
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Row
{
        // simulate a constexpr lambda (not allowed in C++14)
        struct lambda
        {
                Color const to_move;
                int const row;

                constexpr auto operator()(int sq) const noexcept
                {
                        return to_llo(sq, Board::outer_grid).y == (to_move == Color::white ? row : Board::height() - 1 - row);
                }
        };

        template<Color ToMove>
        static constexpr auto init(int row) noexcept
        {
                return make_set_if<Board>(lambda{ToMove, row});
        }

        static constexpr auto N = Board::height();
        using Set = set_type<Board>;
        using table_type = std::array<Set, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Color::black>),
                make_array<N>(init<Color::white>)
        };

public:
        static constexpr auto mask(Color c, int row) noexcept
        {
                assert(row < N);
                return table[static_cast<std::size_t>(c)][static_cast<std::size_t>(row)];
        }
};

template<class Board>
constexpr typename Row<Board>::table_type
Row<Board>::table[];

}       // namespace board
}       // namespace dctl
