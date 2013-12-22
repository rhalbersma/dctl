#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/grid/coordinates.hpp>    // coord_from_sq
#include <dctl/node/side.hpp>           // black, white
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class Row
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                bool const color_;
                int const row_;

                template<class Square>
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        using Grid = typename Square::grid_type;
                        return coord_from_sq(sq).row() == (color_ ? Grid::height - 1 - row_ : row_);
                }
        };

        template<bool Color>
        static constexpr auto init(int row) noexcept
        {
                return Board::copy_if(lambda{Color, row});
        }

        using T = typename Board::set_type;
        static constexpr auto N = Board::height;
        using table_type = std::array<T, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Side::black>),
                make_array<N>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int row) noexcept
        {
                return table[color][static_cast<std::size_t>(row)];
        }
};

template<class Board>
constexpr typename Row<Board>::table_type Row<Board>::table[];

}       // namespace board
}       // namespace dctl
