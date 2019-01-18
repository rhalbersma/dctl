#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>                    // angle, inverse
#include <dctl/core/board/coordinates.hpp>              // to_llo, transform
#include <dctl/core/board/detail/bit_layout.hpp>        // dimensions, InnerGrid, bit_layout
#include <dctl/core/rules/type_traits.hpp>              // width_v, height_v, is_inverted_v, is_orthogonal_jump_v
#include <dctl/core/state/color.hpp>                    // black, white
#include <xstd/bit_set.hpp>                             // bit_set
#include <xstd/cstdlib.hpp>                             // euclidean_div
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
        constexpr static auto orientation = std::min(
                { 0_deg, 90_deg, 180_deg, 270_deg },
                [g = detail::bit_layout{inner_grid, edge}]
                (angle const lhs, angle const rhs) {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        );
        constexpr static auto outer_grid = detail::bit_layout{rotate(inner_grid, orientation), edge};

private:
        constexpr static std::size_t NumBits = outer_grid.size();
        constexpr static std::size_t NumSquares = inner_grid.size();

public:
        constexpr static auto size() noexcept
        {
                return static_cast<int>(NumSquares);
        }

        constexpr static auto bits() noexcept
        {
                return static_cast<int>(NumBits);
        }

        constexpr static auto xyz = boost::alignment::align_up(NumBits, sizeof(xstd::bit_set<NumBits>) * std::numeric_limits<unsigned char>::digits);
        using    set_type = xstd::bit_set<xyz>;
        constexpr static auto abc = set_type::max_ssize();
        using square_type = typename boost::uint_value_t<abc>::least;

        static auto numeric_from_bit(int const n)
        {
                assert(0 <= n); assert(n < bits());
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(int const n)
        {
                assert(0 <= n); assert(n < bits());
                constexpr auto file_label = [](int const f) { return static_cast<char>('a' + f); };
                constexpr auto rank_label = [](int const r) { return 1 + r;                      };
                std::stringstream sstr;
                auto const coord = to_llo(square_from_bit(n), inner_grid);
                sstr << file_label(coord.x) << rank_label(coord.y);
                return sstr.str();
        }

private:
        constexpr static auto bit_from_square_table = []() {
                auto table = std::array<int, NumSquares>{};
                for (auto sq = 0; sq < size(); ++sq) {
                        table[static_cast<std::size_t>(sq)] =
                                transform(sq, inner_grid, outer_grid, inverse(orientation))
                        ;
                }
                return table;
        }();

        constexpr static auto square_from_bit_table = []() {
                auto table = std::array<int, NumBits>{};
                for (auto n = 0; n < bits(); ++n) {
                        table[static_cast<std::size_t>(n)] =
                                transform(n, outer_grid, inner_grid, orientation)
                        ;
                }
                return table;
        }();

public:
        constexpr static auto bit_from_square(int const sq) // Throws: Nothing.
        {
                assert(0 <= sq); assert(sq < size());
                return bit_from_square_table[static_cast<std::size_t>(sq)];
        }

        constexpr static auto square_from_bit(int const n) // Throws: Nothing.
        {
                assert(0 <= n); assert(n < bits());
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
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto squares_filter(UnaryPredicate pred) noexcept
        {
                set_type filter;
                for (auto const n : squares) {
                        if (pred(square_from_bit(n))) {
                                filter.insert(n);
                        }
                }
                return filter;
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto file_table = []() {
                std::array<std::array<set_type, width>, 2> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto f = 0; f < width; ++f) {
                                table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, inner_grid).x == (c == color::white ? f : width - 1 - f);
                                        })
                                ;
                        }
                }
                return table;
        }();

        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto rank_table = []() {
                std::array<std::array<set_type, height>, 2> table;
                for (auto&& c : { color::black, color::white }) {
                        for (auto r = 0; r < height; ++r) {
                                table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)] =
                                        squares_filter([=](int const sq) {
                                                return to_llo(sq, inner_grid).y == (c == color::white ? r : height - 1 - r);
                                        })
                                ;
                        }
                }
                return table;
         }();

public:
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto file(color const c, int const f) // Throws: Nothing.
        {
                assert(0 <= f); assert(f < width);
                return file_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto rank(color const c, int const r) // Throws: Nothing.
        {
                assert(0 <= r); assert(r < height);
                return rank_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)];
        }

        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto promotion(color const c) noexcept
        {
                return rank(c, height - 1);
        }

private:
        XSTD_PP_CONSTEXPR_INTRINSIC_MEM static auto initial_table = []() {
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
        XSTD_PP_CONSTEXPR_INTRINSIC_FUN static auto initial(color const c, int const separation) // Throws: Nothing.
        {
                assert((height - separation) % 2 == 0);
                assert(height % 2 <= separation); assert(separation <= height);
                auto const d = (height - separation) / 2;
                assert(d <= height / 2);
                return initial_table[xstd::to_underlying_type(c)][static_cast<std::size_t>(d)];
        }

        constexpr static auto is_square(coordinates<upper_left> const& coord) noexcept
        {
                return static_cast<bool>((coord.x % 2) ^ (coord.y % 2)) != inner_grid.upper_left_is_square();
        }

        constexpr static auto to_square(coordinates<upper_left> const& coord) noexcept
        {
                return core::to_square(coord, inner_grid);
        }

        constexpr static auto is_onboard(int const sq) noexcept
        {
                return static_cast<unsigned>(sq) < static_cast<unsigned>(set_type::max_ssize());
        }
};

}       // namespace block_adl

using block_adl::rectangular;
using block_adl::basic_board;

}       // namespace dctl::core
