#pragma once
#include <array>                        // array
#include <cassert>                      // assert
#include <cstddef>                      // size_t
#include <dctl/board/mask/copy_if.hpp>  // copy_if
#include <dctl/grid/coordinates.hpp>    // ulo_from_sq
#include <dctl/position/side.hpp>           // black, white
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class Initial
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                bool const color_;
                int const rows_;

                template<class Square>
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        using Grid = typename Square::grid_type;
                        auto const separation = Board::height - 2 * rows_;
                        auto const y = get_y(llo_from_ulo(ulo_from_sq(sq)));
                        auto const min_y = color_ == Side::white ? 0 : Grid::height - (Grid::height - separation) / 2;
                        auto const max_y = color_ == Side::white ? (Grid::height - separation) / 2 : Grid::height;
                        return min_y <= y && y < max_y;
                }
        };

        template<bool Color>
        static constexpr auto init(int rows) noexcept
        {
                return copy_if(Board{}, lambda{Color, rows});
        }

        static constexpr auto N = Board::height / 2 + 1;
        using Set = typename Board::set_type;
        using table_type = std::array<Set, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Side::black>),
                make_array<N>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int separation) noexcept
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation && separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(0 <= rows && rows <= Board::height / 2);
                return table[color][static_cast<std::size_t>(rows)];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type
Initial<Board>::table[];

}       // namespace board
}       // namespace dctl
