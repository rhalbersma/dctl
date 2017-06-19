#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/file_rank.hpp>        // rank
#include <dctl/core/state/color_piece.hpp>      // color, black, white
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {

template<class Board>
class initial
{
        constexpr static auto N = Board::height / 2 + 1;

        constexpr static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, N>, 2>{};
                for (auto const c : { color::black, color::white }) {
                        for (auto d = 0; d < N; ++d) {
                                result[xstd::to_underlying_type(c)][static_cast<std::size_t>(d)] = [=](){
                                        auto accum = set_t<Board>{};
                                        for (auto r = 0; r < d; ++r) {
                                                accum ^= rank<Board>{}(c, r);
                                        }
                                        return accum;
                                }();
                        }
                }
                return result;
        }();
public:
        constexpr auto operator()(color const c, int const separation) const // Throws: Nothing.
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const d = (Board::height - separation) / 2;
                assert(d <= Board::height / 2);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(d)];
        }
};

}       // namespace dctl::core
