#pragma once
#include <dctl/core/board/detail/coordinates.hpp>    // to_llo
#include <dctl/core/board/mask/detail/copy_if.hpp>   // copy_if
#include <dctl/core/state/color_piece.hpp>                 // black, white
#include <dctl/util/fill_array.hpp>          // fill_array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {
namespace board {
namespace mask {

template<class Board, class Color>
class row
{
        static constexpr auto value = fill_array<Board::height>([](int const r) {
                return detail::copy_if<Board>([=](auto const sq) {
                        assert(r < Board::height);
                        return board::detail::to_llo(sq, Board::inner_grid).y == (Color{} == white_c ? r : Board::height - 1 - r);
                });
        });
public:
        constexpr auto operator()(int const r) const noexcept
        {
                assert(r < Board::height);
                return value[static_cast<std::size_t>(r)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl::core
