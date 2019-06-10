#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/coordinates.hpp>      // to_llo, transform
#include <dctl/core/state/color.hpp>            // black, white
#include <xstd/bit_set.hpp>                     // bit_set
#include <xstd/utility.hpp>                     // to_underlying
#include <boost/integer.hpp>                    // uint_value_t
#include <array>                                // array

namespace dctl::core {

template<class Board>
class basic_mask
{
public:
        using    set_type = xstd::bit_set<xstd::bit_set<Board::bits()>::capacity()>;
        using square_type = typename boost::uint_value_t<set_type::max_ssize()>::least;

        constexpr static auto squares = []() {
                set_type table;
                for (auto sq = 0; sq < Board::size(); ++sq) {
                        table.insert(Board::embedding0(sq));
                }
                return table;
        }();

private:
        template<class UnaryPredicate>
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto squares_filter(UnaryPredicate pred) noexcept
        {
                set_type filter;
                for (auto const n : squares) {
                        if (pred(Board::numeric0(n))) {
                                filter.insert(n);
                        }
                }
                return filter;
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto file_table = []() {
                std::array<std::array<set_type, Board::width>, xstd::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto f = 0; f < Board::width; ++f) {
                                table[xstd::to_underlying(c)][static_cast<std::size_t>(f)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, Board::inner_grid).x == (c == color::white ? f : Board::width - 1 - f);
                                        })
                                ;
                        }
                }
                return table;
        }();

        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto rank_table = []() {
                std::array<std::array<set_type, Board::height>, xstd::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto r = 0; r < Board::height; ++r) {
                                table[xstd::to_underlying(c)][static_cast<std::size_t>(r)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, Board::inner_grid).y == (c == color::white ? r : Board::height - 1 - r);
                                        })
                                ;
                        }
                }
                return table;
         }();

public:
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto file(color const c, int const f) // Throws: Nothing.
        {
                assert(0 <= f); assert(f < Board::width);
                return file_table[xstd::to_underlying(c)][static_cast<std::size_t>(f)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto rank(color const c, int const r) // Throws: Nothing.
        {
                assert(0 <= r); assert(r < Board::height);
                return rank_table[xstd::to_underlying(c)][static_cast<std::size_t>(r)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto promotion(color const c) noexcept
        {
                return rank(c, Board::height - 1);
        }

private:
        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto initial_table = []() {
                constexpr auto N = Board::height / 2 + 1;
                std::array<std::array<set_type, N>, xstd::to_underlying(color::size)> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto d = 0; d < N; ++d) {
                                table[xstd::to_underlying(c)][static_cast<std::size_t>(d)] = [=]() {
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
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto initial(color const c, int const separation) // Throws: Nothing.
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const d = (Board::height - separation) / 2;
                assert(d <= Board::height / 2);
                return initial_table[xstd::to_underlying(c)][static_cast<std::size_t>(d)];
        }
};

}       // namespace dctl::core
