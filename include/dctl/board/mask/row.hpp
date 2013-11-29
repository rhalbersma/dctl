#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/grid/predicates.hpp>     // is_row
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

                constexpr lambda(bool color, int row) noexcept
                :
                        color_{color},
                        row_{row}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_row{}(color_, row_, sq);
                }
        };

        template<bool Color>
        static constexpr auto init(int row) noexcept
        {
                return Board::copy_if(lambda{Color, row});
        }

        static constexpr std::array<typename Board::bit_type, 20> table[] =
        {
                make_array<20>(init<Side::black>),
                make_array<20>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int row) noexcept
        {
                return table[color][static_cast<std::size_t>(row)];
        }
};

template<class Board>
constexpr std::array<typename Board::bit_type, 20> Row<Board>::table[];

}       // namespace board
}       // namespace dctl

