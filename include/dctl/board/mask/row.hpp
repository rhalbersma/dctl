#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/board/mask/copy_if.hpp>  // copy_if
#include <dctl/grid/coordinates.hpp>    // ulo_from_sq
#include <dctl/position/side.hpp>           // black, white
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
                        return get_y(llo_from_ulo(ulo_from_sq(sq))) == (color_ == Side::white ? row_ : Grid::height - 1 - row_);
                }
        };

        template<bool Color>
        static constexpr auto init(int row) noexcept
        {
                return copy_if(Board{}, lambda{Color, row});
        }

        static constexpr auto N = Board::height;
        using Set = typename Board::set_type;
        using table_type = std::array<Set, N>;

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
constexpr typename Row<Board>::table_type
Row<Board>::table[];

}       // namespace board
}       // namespace dctl
