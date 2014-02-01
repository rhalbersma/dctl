#pragma once
#include <dctl/angle/detail/abs_remainder.hpp>
#include <dctl/board/mask/copy_if.hpp>  // copy_if
#include <dctl/grid/coordinates.hpp>

namespace dctl {
namespace board {

template<class Board>
class JumpGroup
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                int from_bit_;

                template<class Square>
                constexpr auto operator()(Square const& dest_sq) const noexcept
                {
                        auto const from_sq = grid::ulo::Square<typename Board::external_grid>{Board::square_from_bit(from_bit_)};
                        auto const from_coord = coord_from_sq(from_sq);
                        auto const dest_coord = coord_from_sq(dest_sq);
                        auto const delta_row = dctl::detail::abs_remainder(from_coord.row() - dest_coord.row(), 4);
                        auto const delta_col = dctl::detail::abs_remainder(from_coord.col() - dest_coord.col(), 4);
                        return
                                (delta_row == 0 && delta_col == 0) ||
                                (delta_row == 2 && delta_col == 2)
                        ;
                }
        };

        static constexpr auto init(int n) noexcept
        {
                return copy_if(Board{}, lambda{n});
        }

        static constexpr auto N = 4;
        using Set = typename Board::set_type;
        using table_type = std::array<Set, N>;
        using Grid = typename Board::internal_grid;

        static constexpr table_type table =
        {{
                init(0),
                init(1),
                init(Grid::left_down + (Grid::ul_parity ? 2 : 0)),
                init(Grid::right_down)
        }};

public:
        static constexpr auto mask(int n) noexcept
        {
                return table[static_cast<std::size_t>(n)];
        }
};

template<class Board>
constexpr typename JumpGroup<Board>::table_type
JumpGroup<Board>::table;

}       // namespace board
}       // namespace dctl
