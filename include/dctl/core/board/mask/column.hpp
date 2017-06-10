#pragma once
#include <dctl/core/board/detail/coordinates.hpp>       // to_llo
#include <dctl/core/board/mask/detail/copy_if.hpp>      // copy_if
#include <dctl/core/state/color_piece.hpp>              // black, white
#include <array>                                        // array
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t

namespace dctl::core {
namespace board {
namespace mask {

template<class Board, class Color>
class column
{
        static constexpr auto value = []() {
                std::array<set_t<Board>, Board::width> table{};
                for (auto c = 0; c < Board::width; ++c) {
                        table[static_cast<std::size_t>(c)] = detail::copy_if<Board>([=](auto const sq){
                                assert(0 <= c); assert(c < Board::width);
                                return board::detail::to_llo(sq, Board::inner_grid).x == (Color{} == white_c ? c : Board::width - 1 - c);
                        });
                }
                return table;
        }();
public:
        constexpr auto operator()(int const c) const noexcept
        {
                assert(static_cast<std::size_t>(c) < Board::width);
                return value[static_cast<std::size_t>(c)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl::core
