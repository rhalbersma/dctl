#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/detail/set_filter.hpp>        // set_filter
#include <dctl/core/board/coordinates.hpp>              // to_llo
#include <dctl/core/state/color_piece.hpp>              // color, black, white
#include <xstd/type_traits.hpp>                         // to_underlying_type
#include <array>                                        // array
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t

namespace dctl::core {

template<class Board>
class file
{
        constexpr static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, Board::width>, 2>{};
                for (auto const c : { color::black, color::white }) {
                        for (auto f = 0; f < Board::width; ++f) {
                                result[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)] =
                                        detail::set_filter<Board>([=](auto const sq) {
                                                return to_llo(sq, Board::inner_grid).x == (c == color::white ? f : Board::width - 1 - f);
                                        })
                                ;
                        }
                }
                return result;
        }();
public:
        constexpr auto operator()(color const c, int const f) const // Throws: Nothing.
        {
                assert(static_cast<std::size_t>(f) < Board::width);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)];
        }
};

template<class Board>
class rank
{
        constexpr static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, Board::height>, 2>{};
                for (auto const c : { color::black, color::white }) {
                        for (auto r = 0; r < Board::height; ++r) {
                                result[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)] =
                                        detail::set_filter<Board>([=](auto const sq) {
                                                return to_llo(sq, Board::inner_grid).y == (c == color::white ? r : Board::height - 1 - r);
                                        })
                                ;
                        }
                }
                return result;
        }();
public:
        constexpr auto operator()(color const c, int const r) const // Throws: Nothing.
        {
                assert(static_cast<std::size_t>(r) < Board::height);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)];
        }
};

}       // namespace dctl::core
