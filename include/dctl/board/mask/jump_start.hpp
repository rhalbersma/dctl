#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/angle.hpp>               // Angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/grid/coordinates.hpp>    // coord_from_sq
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class JumpStart
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                int const segment_;

                template<class Square>
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        using Grid = typename Square::grid_type;
                        auto const alpha = rotate(segment_ * theta + beta, Board::orientation);
                        auto const offset = is_diagonal(alpha) ? 2 : 4;
                        auto const coord = coord_from_sq(sq);
                        auto const min_row = is_up(alpha) ? offset : 0;
                        auto const max_row = Grid::height - (is_down(alpha)? offset : 0);
                        auto const min_col = is_left(alpha) ? offset : 0;
                        auto const max_col = Grid::width - (is_right(alpha) ? offset : 0);
                        return
                                (min_row <= coord.row() && coord.row() < max_row) &&
                                (min_col <= coord.col() && coord.col() < max_col)
                        ;
                }
        };

        static constexpr auto init(int segment) noexcept
        {
                return Board::copy_if(lambda{segment});
        }

        using T = typename Board::set_type;
        static constexpr auto N     = Board::is_orthogonal_captures ?      4 :      8;
        static constexpr auto theta = Board::is_orthogonal_captures ? 90_deg : 45_deg;
        static constexpr auto beta  = Board::is_orthogonal_captures ? 45_deg :  0_deg;
        using table_type = std::array<T, N>;

        static constexpr table_type table = make_array<N>(init);

public:
        static constexpr auto mask(Angle const& alpha) noexcept
        {
                auto const segment = (alpha - beta) / theta;
                return table[static_cast<std::size_t>(segment)];
        }
};

template<class Board>
constexpr Angle JumpStart<Board>::theta;

template<class Board>
constexpr Angle JumpStart<Board>::beta;

template<class Board>
constexpr typename JumpStart<Board>::table_type JumpStart<Board>::table;

}       // namespace board
}       // namespace dctl
