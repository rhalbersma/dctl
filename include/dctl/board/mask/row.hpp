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

template<class Board, class color>
class row
{
        struct init
        {
                struct is_row
                {
                        int const row_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(int const sq) const noexcept
                        {
                                assert(row_ < Board::height);
                                return board::detail::to_llo(sq, Board::inner_grid).y == (color{} == white_c ? row_ : Board::height - 1 - row_);
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(int const row) const noexcept
                {
                        return detail::copy_if<Board>(is_row{row});
                }
        };

        using value_type = std::array<set_t<Board>, Board::height>;

        static constexpr value_type value = fill_array<Board::height>(init{});

public:
        constexpr auto operator()(int const row) const noexcept
        {
                assert(row < Board::height);
                return value[static_cast<std::size_t>(row)];
        }
};

template<class Board, class color>
constexpr value_t<row<Board, color>>
row<Board, color>::value;

}       // namespace mask
}       // namespace board
}       // namespace dctl
