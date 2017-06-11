#pragma once
#include <dctl/core/board/file_rank.hpp>        // rank
#include <dctl/core/state/color_piece.hpp>      // black, white
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {

template<class Board, class Color>
class initial
{
        constexpr static auto N = Board::height / 2 + 1;
        constexpr static auto table = []() {
                auto result = std::array<set_t<Board>, N>{};
                for (auto d = 0; d < N; ++d) {
                        result[static_cast<std::size_t>(d)] = [=](){
                                auto accum = set_t<Board>{};
                                for (auto r = 0; r < d; ++r) {
                                        accum ^= rank<Board>{}(Color{}, r);
                                }
                                return accum;
                        }();
                }
                return result;
        }();
public:
        constexpr auto operator()(int const separation) const // Throws: Nothing.
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(rows <= Board::height / 2);
                return table[static_cast<std::size_t>(rows)];
        }
};

}       // namespace dctl::core
