#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/config.hpp>
#include <dctl/core/board/angle.hpp>                    // angle, inverse
#include <dctl/core/board/coordinates.hpp>              // to_llo, transform
#include <dctl/core/board/detail/bit_layout.hpp>        // dimensions, InnerGrid, bit_layout
#include <dctl/core/rules/type_traits.hpp>              // width_v, height_v, is_inverted_v, is_orthogonal_jump_v
#include <dctl/core/state/color_piece.hpp>              // black, white
#include <xstd/cstdlib.hpp>                             // euclidean_div
#include <xstd/int_set.hpp>                             // int_set
#include <boost/align/align_up.hpp>                     // align_up
#include <boost/integer.hpp>                            // uint_value_t
#include <algorithm>                                    // min
#include <array>                                        // array
#include <cstddef>                                      // size_t
#include <iomanip>                                      // setfill
#include <limits>                                       // digits
#include <sstream>                                      // stringstream

namespace dctl::core {
namespace block_adl {

template<int Width, int Height, bool IsInverted = false, bool IsOrthogonalJump = false>
struct rectangular
{
        constexpr static auto width = Width;
        constexpr static auto height = Height;
        constexpr static auto is_inverted = IsInverted;
        constexpr static auto is_orthogonal_jump = IsOrthogonalJump;
};

template<class Geometry>
class basic_board
{
public:
        using type = basic_board;
        constexpr static auto width              = width_v<Geometry>;
        constexpr static auto height             = height_v<Geometry>;
        constexpr static auto is_inverted        = is_inverted_v<Geometry>;
        constexpr static auto is_orthogonal_jump = is_orthogonal_jump_v<Geometry>;

        constexpr static auto edge = is_orthogonal_jump ? 2 : 1;
        constexpr static auto inner_grid = detail::InnerGrid{detail::dimensions{width, height, is_inverted}};
        constexpr static angle orientation = std::min(
                { 0_deg, 90_deg, 180_deg, 270_deg },
                [g = detail::bit_layout{inner_grid, edge}]
                (angle const lhs, angle const rhs) {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        );
        constexpr static auto outer_grid = detail::bit_layout{rotate(inner_grid, orientation), edge};

private:
        constexpr static auto NumBits = outer_grid.size();
        constexpr static auto NumSquares = inner_grid.size();

public:
        constexpr static auto size() noexcept
        {
                return NumSquares;
        }

        constexpr static auto bits() noexcept
        {
                return NumBits;
        }

        using    set_type = xstd::int_set<static_cast<int>(boost::alignment::align_up(NumBits, sizeof(xstd::int_set<NumBits>) * std::numeric_limits<unsigned char>::digits))>;
        using square_type = typename boost::uint_value_t<set_type{}.max_size()>::least;

        static auto numeric_from_bit(int const n)
        {
                assert(0 <= n); assert(n < NumBits);
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(int const n)
        {
                assert(0 <= n); assert(n < NumBits);
                constexpr auto file_label = [](auto const f) { return static_cast<char>('a' + f); };
                constexpr auto rank_label = [](auto const r) { return 1 + r;                      };
                std::stringstream sstr;
                auto const coord = to_llo(square_from_bit(n), inner_grid);
                sstr << file_label(coord.x) << rank_label(coord.y);
                return sstr.str();
        }

private:
        constexpr static auto bit_from_square_table = []() {
                auto table = std::array<int, static_cast<std::size_t>(NumSquares)>{};
                for (auto sq = 0; sq < NumSquares; ++sq) {
                        table[static_cast<std::size_t>(sq)] =
                                transform(sq, inner_grid, outer_grid, inverse(orientation))
                        ;
                }
                return table;
        }();

        constexpr static auto square_from_bit_table = []() {
                auto table = std::array<int, static_cast<std::size_t>(NumBits)>{};
                for (auto n = 0; n < NumBits; ++n) {
                        table[static_cast<std::size_t>(n)] =
                                transform(n, outer_grid, inner_grid, orientation)
                        ;
                }
                return table;
        }();

public:
        constexpr static auto bit_from_square(int const sq) // Throws: Nothing.
        {
                assert(0 <= sq); assert(sq < NumSquares);
                return bit_from_square_table[static_cast<std::size_t>(sq)];
        }

        constexpr static auto square_from_bit(int const n) // Throws: Nothing.
        {
                assert(0 <= n); assert(n < NumBits);
                return square_from_bit_table[static_cast<std::size_t>(n)];
        }

public:
        constexpr static auto squares = []() {
                set_type table;
                for (auto sq = 0; sq < size(); ++sq) {
                        table.insert(bit_from_square(sq));
                }
                return table;
        }();

private:
        template<class UnaryPredicate>
        XSTD_PP_CONSTEXPR_INTRINSIC static auto squares_filter(UnaryPredicate pred) noexcept
        {
                set_type filter;
                for (auto const n : squares) {
                        if (pred(square_from_bit(n))) {
                                filter.insert(n);
                        }
                }
                return filter;
        }

        DCTL_PP_CONSTEXPR_INTRINSIC static auto file_table = []() {
                std::array<std::array<set_type, width>, 2> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto f = 0; f < width; ++f) {
                                table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)] =
                                        squares_filter([=](auto const sq) {
                                                return to_llo(sq, inner_grid).x == (c == color::white ? f : width - 1 - f);
                                        })
                                ;
                        }
                }
                return table;
        }();

        DCTL_PP_CONSTEXPR_INTRINSIC static auto rank_table = []() {
                std::array<std::array<set_type, height>, 2> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto r = 0; r < height; ++r) {
                                table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)] =
                                        squares_filter([=](auto const sq) {
                                                return to_llo(sq, inner_grid).y == (c == color::white ? r : height - 1 - r);
                                        })
                                ;
                        }
                }
                return table;
         }();

        constexpr static auto theta        = is_orthogonal_jump ? 45_deg : 90_deg;
        constexpr static auto beta         = is_orthogonal_jump ?  0_deg : 45_deg;
        constexpr static auto num_segments = is_orthogonal_jump ?      8 :      4;

        DCTL_PP_CONSTEXPR_INTRINSIC static auto jump_start_table = []() {
                std::array<set_type, num_segments> table;
                for (auto segment = 0; segment < num_segments; ++segment) {
                        table[static_cast<std::size_t>(segment)] = squares_filter([=](auto const sq) {
                                auto const alpha = rotate(segment * theta + beta, inverse(orientation));
                                auto const offset = is_diagonal(alpha) ? 2 : 4;
                                auto const min_x = is_left(alpha) ? offset : 0;
                                auto const max_x = width - (is_right(alpha) ? offset : 0);
                                auto const min_y = is_up(alpha) ? offset : 0;
                                auto const max_y = height - (is_down(alpha) ? offset : 0);
                                auto const coord = to_ulo(sq, inner_grid);
                                return
                                        (min_x <= coord.x && coord.x < max_x) &&
                                        (min_y <= coord.y && coord.y < max_y)
                                ;
                        });
                }
                return table;
        }();

        template<int FromSquare>
        XSTD_PP_CONSTEXPR_INTRINSIC static auto init_jump_group() noexcept
        {
                return squares_filter([](auto const dest_sq) {
                        auto const from_coord = to_llo(FromSquare, inner_grid);
                        auto const dest_coord = to_llo(dest_sq   , inner_grid);
                        auto const delta_x = xstd::euclidean_div(from_coord.x - dest_coord.x, 4).rem;
                        auto const delta_y = xstd::euclidean_div(from_coord.y - dest_coord.y, 4).rem;
                        return
                                (delta_x == 0 && delta_y == 0) ||
                                (delta_x == 2 && delta_y == 2)
                        ;
                });
        }

        DCTL_PP_CONSTEXPR_INTRINSIC static auto jump_group_table = std::array<set_type, 4>
        {{
                init_jump_group<inner_grid.edge_le() + 0>(),
                init_jump_group<inner_grid.edge_le() + 1>(),
                init_jump_group<inner_grid.edge_lo() + 0>(),
                init_jump_group<inner_grid.edge_lo() + 1>()
        }};

public:
        XSTD_PP_CONSTEXPR_INTRINSIC static auto file(color const c, int const f) // Throws: Nothing.
        {
                assert(0 <= f); assert(f < width);
                return file_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC static auto rank(color const c, int const r) // Throws: Nothing.
        {
                assert(0 <= r); assert(r < height);
                return rank_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC static auto promotion(color const c) noexcept
        {
                return rank(c, height - 1);
        }

        XSTD_PP_CONSTEXPR_INTRINSIC static auto jump_start(angle const alpha) // Throws: Nothing.
        {
                auto const segment = (alpha - beta) / theta;
                assert(0 <= segment); assert(segment < num_segments);
                return jump_start_table[static_cast<std::size_t>(segment)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC static auto jump_group(int const j) // Throws: Nothing.
        {
                assert(0 <= j); assert(j < 4);
                return jump_group_table[static_cast<std::size_t>(j)];
        }

private:
        DCTL_PP_CONSTEXPR_INTRINSIC static auto initial_table = []() {
                constexpr auto N = height / 2 + 1;
                std::array<std::array<set_type, N>, 2> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto d = 0; d < N; ++d) {
                                table[xstd::to_underlying_type(c)][static_cast<std::size_t>(d)] = [=]() {
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
        XSTD_PP_CONSTEXPR_INTRINSIC static auto initial(color const c, int const separation) // Throws: Nothing.
        {
                assert((height - separation) % 2 == 0);
                assert(height % 2 <= separation); assert(separation <= height);
                auto const d = (height - separation) / 2;
                assert(d <= height / 2);
                return initial_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(d)];
        }

        constexpr static auto is_square(coordinates<upper_left> const& coord) noexcept
        {
                return ((coord.x % 2) ^ (coord.y % 2)) != inner_grid.upper_left_is_square();
        }

        constexpr static auto to_square(coordinates<upper_left> const& coord) noexcept
        {
                return core::to_square(coord, inner_grid);
        }

        constexpr static auto is_onboard(int const sq) noexcept
        {
                return static_cast<unsigned>(sq) < static_cast<unsigned>(set_type{}.max_size());
        }
};

}       // namespace block_adl

using block_adl::rectangular;
using block_adl::basic_board;

}       // namespace dctl::core
