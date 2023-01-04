#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>                    // angle, inverse
#include <dctl/core/board/coordinates.hpp>              // to_llo, transform
#include <dctl/core/board/detail/bit_layout.hpp>        // dimensions, InnerGrid, bit_layout
#include <dctl/core/rules/type_traits.hpp>              // width_v, height_v, coloring_v, is_orthogonal_jumps_v
#include <dctl/core/state/color.hpp>                    // black, white
#include <xstd/cstdlib.hpp>                             // euclidean_div
#include <algorithm>                                    // min
#include <array>                                        // array
#include <cstddef>                                      // size_t
#include <iomanip>                                      // setfill
#include <limits>                                       // digits
#include <sstream>                                      // stringstream

namespace dctl::core {
namespace block_adl {

template<int Width, int Height, int Coloring = 1, bool IsOrthogonalJumps = false>
struct rectangular
{
        static constexpr auto width = Width;
        static constexpr auto height = Height;
        static constexpr auto coloring = Coloring;
        static constexpr auto is_orthogonal_jumps = IsOrthogonalJumps;
};

template<class Geometry>
class basic_board
{
public:
        using type = basic_board;
        static constexpr auto width               = width_v<Geometry>;
        static constexpr auto height              = height_v<Geometry>;
        static constexpr auto coloring            = coloring_v<Geometry>;
        static constexpr auto is_orthogonal_jumps = is_orthogonal_jumps_v<Geometry>;

        static constexpr auto edge = is_orthogonal_jumps ? 2 : 1;
        static constexpr auto inner_grid = detail::InnerGrid{detail::dimensions{width, height, coloring}};
        static constexpr auto orientation = std::min(
                { 0_deg, 90_deg, 180_deg, 270_deg },
                [g = detail::bit_layout{inner_grid, edge}]
                (angle const lhs, angle const rhs) {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        );
        static constexpr auto outer_grid = detail::bit_layout{rotate(inner_grid, orientation), edge};

private:
        static constexpr std::size_t NumBits = outer_grid.size();
        static constexpr std::size_t NumSquares = inner_grid.size();

public:
        static constexpr auto size() noexcept
        {
                return static_cast<int>(NumSquares);
        }

        static constexpr auto bits() noexcept
        {
                return static_cast<int>(NumBits);
        }

        static auto numeric_from_bit(int const n)
        {
                assert(0 <= n); assert(n < bits());
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << numeric1(n);
                return sstr.str();
        }

        static auto algebraic_from_bit(int const n)
        {
                assert(0 <= n); assert(n < bits());
                constexpr auto file_label = [](int const f) { return static_cast<char>('a' + f); };
                constexpr auto rank_label = [](int const r) { return 1 + r;                      };
                std::stringstream sstr;
                auto const coord = to_llo(numeric0(n), inner_grid);
                sstr << file_label(coord.x) << rank_label(coord.y);
                return sstr.str();
        }

private:
        static constexpr auto numeric0_table = []() {
                auto table = std::array<int, NumBits>{};
                for (auto n = 0; n < bits(); ++n) {
                        table[static_cast<std::size_t>(n)] =
                                transform(n, outer_grid, inner_grid, orientation)
                        ;
                }
                return table;
        }();

        static constexpr auto embedding0_table = []() {
                auto table = std::array<int, NumSquares>{};
                for (auto sq = 0; sq < size(); ++sq) {
                        table[static_cast<std::size_t>(sq)] =
                                transform(sq, inner_grid, outer_grid, inverse(orientation))
                        ;
                }
                return table;
        }();

public:
        static constexpr auto numeric0(int const n) // Throws: Nothing.
        {
                assert(0 <= n); assert(n < bits());
                return numeric0_table[static_cast<std::size_t>(n)];
        }

        static constexpr auto numeric1(int const n) // Throws: Nothing.
        {
                assert(0 <= n); assert(n < bits());
                return numeric0(n) + 1;
        }

        static constexpr auto embedding0(int const sq) // Throws: Nothing.
        {
                assert(0 <= sq); assert(sq < size());
                return embedding0_table[static_cast<std::size_t>(sq)];
        }

        static constexpr auto embedding1(int const sq) // Throws: Nothing.
        {
                assert(1 <= sq); assert(sq <= size());
                return embedding0(sq - 1);
        }

        static constexpr auto is_square(coordinates<upper_left> const& coord) noexcept
        {
                return static_cast<bool>((coord.x % 2) ^ (coord.y % 2)) != inner_grid.upper_left_is_square();
        }

        static constexpr auto is_square(coordinates<lower_left> const& coord) noexcept
        {
                return static_cast<bool>((coord.x % 2) ^ (coord.y % 2)) != inner_grid.lower_left_is_square();
        }

        static constexpr auto to_square(coordinates<upper_left> const& coord) noexcept
        {
                return core::to_square(coord, inner_grid);
        }

        static constexpr auto to_square(coordinates<lower_left> const& coord) noexcept
        {
                return core::to_square(coord, inner_grid);
        }

        static constexpr auto is_onboard(int const sq) noexcept
        {
                return static_cast<unsigned>(sq) < static_cast<unsigned>(NumBits);
        }
};

}       // namespace block_adl

using block_adl::rectangular;
using block_adl::basic_board;

}       // namespace dctl::core
