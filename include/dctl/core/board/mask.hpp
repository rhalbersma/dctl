#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/coordinates.hpp>      // to_llo, transform
#include <dctl/core/state/color.hpp>            // black, white
#include <xstd/bit_set.hpp>                     // bit_set
#include <boost/integer.hpp>                    // uint_value_t
#include <array>                                // array
#include <utility>                              // to_underlying

namespace dctl::core {

template<class Board>
class basic_mask
{
public:
        using    set_type = xstd::bit_set_fast<Board::bits()>;
        using square_type = typename boost::uint_value_t<set_type::max_size()>::least;

        static constexpr auto squares = []() {
                set_type table;
                for (auto sq = 0; sq < Board::size(); ++sq) {
                        table.add(Board::embedding0(sq));
                }
                return table;
        }();

private:
        template<class UnaryPredicate>
        static constexpr auto squares_filter(UnaryPredicate pred) noexcept
        {
                set_type filter;
                for (auto const n : squares) {
                        if (pred(Board::numeric0(n))) {
                                filter.add(n);
                        }
                }
                return filter;
        }

        static constexpr auto file_table = []() {
                std::array<std::array<set_type, Board::width>, std::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto f = 0; f < Board::width; ++f) {
                                table[std::to_underlying(c)][static_cast<std::size_t>(f)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, Board::inner_grid).x == (c == color::white ? f : Board::width - 1 - f);
                                        })
                                ;
                        }
                }
                return table;
        }();

        static constexpr auto rank_table = []() {
                std::array<std::array<set_type, Board::height>, std::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto r = 0; r < Board::height; ++r) {
                                table[std::to_underlying(c)][static_cast<std::size_t>(r)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, Board::inner_grid).y == (c == color::white ? r : Board::height - 1 - r);
                                        })
                                ;
                        }
                }
                return table;
         }();

public:
        static constexpr auto file(color const c, int const f) // Throws: Nothing.
        {
                assert(0 <= f); assert(f < Board::width);
                return file_table[std::to_underlying(c)][static_cast<std::size_t>(f)];
        }

        static constexpr auto rank(color const c, int const r) // Throws: Nothing.
        {
                assert(0 <= r); assert(r < Board::height);
                return rank_table[std::to_underlying(c)][static_cast<std::size_t>(r)];
        }

        static constexpr auto promotion(color const c) noexcept
        {
                return rank(c, Board::height - 1);
        }

private:
        static constexpr auto initial_table = []() {
                constexpr auto N = Board::height / 2 + 1;
                std::array<std::array<set_type, N>, std::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto d = 0; d < N; ++d) {
                                table[std::to_underlying(c)][static_cast<std::size_t>(d)] = [=]() {
                                        set_type accum;
                                        for (auto r = 0; r < d; ++r) {
                                                accum ^= rank(c, r);
                                        }
                                        return accum;
                                }();
                        }
                }
                return table;
        }();

public:
        static constexpr auto initial(color const c, int const separation) // Throws: Nothing.
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const d = (Board::height - separation) / 2;
                assert(d <= Board::height / 2);
                return initial_table[std::to_underlying(c)][static_cast<std::size_t>(d)];
        }
};

}       // namespace dctl::core
