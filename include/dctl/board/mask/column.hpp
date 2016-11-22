#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/board/mask/detail/copy_if.hpp>   // copy_if
#include <dctl/color_piece.hpp>                 // black, white
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t, value_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {
namespace mask {

template<class Board, class Color>
class column
{
        struct init
        {
                struct is_column
                {
                        int const column_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(int const sq) const noexcept
                        {
                                assert(column_ < Board::width);
                                return board::detail::to_llo(sq, Board::inner_grid).x == (Color{} == white_c ? column_ : Board::width - 1 - column_);
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(int const column) const noexcept
                {
                        return detail::copy_if<Board>(is_column{column});
                }
        };

        using value_type = std::array<set_t<Board>, Board::width>;

        static constexpr value_type value = fill_array<Board::width>(init{});

public:
        constexpr auto operator()(int const column) const noexcept
        {
                assert(column < Board::width);
                return value[static_cast<std::size_t>(column)];
        }
};

template<class Board, class Color>
constexpr value_t<column<Board, Color>>
column<Board, Color>::value;

}       // namespace mask
}       // namespace board
}       // namespace dctl
