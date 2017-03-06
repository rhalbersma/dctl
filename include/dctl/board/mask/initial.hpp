#pragma once
#include <dctl/board/mask/row.hpp>      // row
#include <dctl/color_piece.hpp>         // black, white
#include <dctl/utility/fill_array.hpp>  // fill_array
#include <cassert>                      // assert
#include <cstddef>                      // size_t

namespace dctl {
namespace board {
namespace mask {

template<class Board, class Color>
class initial
{
        static constexpr auto N = Board::height / 2 + 1;
        static constexpr auto value = fill_array<N>([](int const rows){
                set_t<Board> result {};
                for (auto r = 0; r < rows; ++r)
                        result ^= row<Board, Color>{}(r);
                return result;
        });
public:
        constexpr auto operator()(int const separation) const
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(rows <= Board::height / 2);
                return value[static_cast<std::size_t>(rows)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl
