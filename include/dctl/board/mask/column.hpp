#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/grid/predicates.hpp>     // is_column
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

                constexpr lambda(bool color, int column) noexcept
                :
                         color_{color},
                         column_{column}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_column{}(color_, column_, sq);
                }
        };

        template<bool Color>
        static constexpr auto init(int column) noexcept
        {
                return Board::copy_if(lambda{Color, column});
        }

        static constexpr std::array<typename Board::bit_type, 20> table[] =
        {
                make_array<20>(init<Side::black>),
                make_array<20>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int column) noexcept
        {
                return table[color][static_cast<std::size_t>(column)];
        }
};

template<class Board>
constexpr std::array<typename Board::bit_type, 20> Column<Board>::table[];

}       // namespace board
}       // namespace dctl

