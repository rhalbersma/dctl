#pragma once
#include <dctl/grid/coordinates.hpp>    // Square

namespace dctl {
namespace board {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class UnaryPredicate>
constexpr auto copy_if(Board const& /* b */, UnaryPredicate pred) noexcept
{
        using Grid = typename Board::external_grid;
        using Set = typename Board::set_type;

        Set result{};
        for (auto sq = 0; sq != Grid::size; ++sq)
                if (pred(grid::ulo::Square<Grid>{sq}))
                        result.set(Board::bit_from_square(sq));
        return result;
}

}       // namespace board
}       // namespace dctl
