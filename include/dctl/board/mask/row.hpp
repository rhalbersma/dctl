#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/board/mask/detail/copy_if.hpp>   // copy_if
#include <dctl/color_piece.hpp>                 // black, white
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {
namespace mask {

template<class Board, class Color>
class row
{
        static constexpr auto value = fill_array<Board::height>([](int const row) {
                return detail::copy_if<Board>([=](auto const sq) {
                        assert(row < Board::height);
                        return board::detail::to_llo(sq, Board::inner_grid).y == (Color{} == white_c ? row : Board::height - 1 - row);
                });
        });
public:
        constexpr auto operator()(int const row) const noexcept
        {
                assert(row < Board::height);
                return value[static_cast<std::size_t>(row)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl
