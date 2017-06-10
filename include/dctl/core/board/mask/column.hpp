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
class column
{
        static constexpr auto value = fill_array<Board::width>([](int const c){
                return detail::copy_if<Board>([=](auto const sq){
                        assert(c < Board::width);
                        return board::detail::to_llo(sq, Board::inner_grid).x == (Color{} == white_c ? c : Board::width - 1 - c);
                });
        });
public:
        constexpr auto operator()(int const c) const noexcept
        {
                assert(c < Board::width);
                return value[static_cast<std::size_t>(c)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl::core
