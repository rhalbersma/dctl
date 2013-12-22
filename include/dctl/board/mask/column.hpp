#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/grid/coordinates.hpp>    // coord_from_sq
#include <dctl/node/side.hpp>           // black, white
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class Column
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                bool const color_;
                int const column_;

                template<class Square>
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        using Grid = typename Square::grid_type;
                        return coord_from_sq(sq).col() == (color_ ? Grid::width - 1 - column_ : column_);
                }
        };

        template<bool Color>
        static constexpr auto init(int column) noexcept
        {
                return Board::copy_if(lambda{Color, column});
        }

        using T = typename Board::set_type;
        static constexpr auto N = Board::width;
        using table_type = std::array<T, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Side::black>),
                make_array<N>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int column) noexcept
        {
                return table[color][static_cast<std::size_t>(column)];
        }
};

template<class Board>
constexpr typename Column<Board>::table_type Column<Board>::table[];

}       // namespace board
}       // namespace dctl
