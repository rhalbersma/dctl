#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/color_piece.hpp>                       // black, white
#include <dctl/mask/detail/copy_if.hpp>         // copy_if
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t, value_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace mask {

template<class Board, class Color>
class row
{
        struct init
        {
                struct is_row
                {
                        std::size_t const row_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                assert(row_ < Board::height);
                                return board::detail::to_llo(sq, Board::inner_grid).y == (Color{} == white_type{} ? row_ : Board::height - 1 - row_);
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(std::size_t const row) const noexcept
                {
                        return detail::copy_if<Board>(is_row{row});
                }
        };

        using value_type = std::array<set_t<Board>, Board::height>;

        static constexpr value_type value = fill_array<Board::height>(init{});

public:
        constexpr auto operator()(std::size_t const row) const noexcept
        {
                assert(row < Board::height);
                return value[row];
        }
};

template<class Board, class Color>
constexpr value_t<row<Board, Color>>
row<Board, Color>::value;

}       // namespace mask
}       // namespace dctl
