#pragma once
#include <dctl/board/coordinates.hpp>           // ulo_from_sq
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/position/color.hpp>              // black, white
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
                bool const color_;
                int const row_;

                constexpr auto operator()(int sq) const noexcept
                {
                        return to_llo(sq, Board::outer_grid).y == (color_ == Color::white ? row_ : Board::height() - 1 - row_);
                }
        };

        template<bool Color>
        static constexpr auto init(int row) noexcept
        {
                return make_set_if<Board>(lambda{Color, row});
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
        static constexpr auto mask(bool color, int row) noexcept
        {
                assert(row < N);
                return table[color][static_cast<std::size_t>(row)];
        }
};

template<class Board>
constexpr typename Row<Board>::table_type
Row<Board>::table[];

}       // namespace board
}       // namespace dctl
