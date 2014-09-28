#pragma once
#include <dctl/grid/coordinates.hpp>    // Square
#include <dctl/type_traits.hpp>

namespace dctl {
namespace board {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class Set = set_type_t<Board>, class UnaryPredicate>
constexpr auto make_set_if(UnaryPredicate pred) noexcept
{
        using Grid = typename Board::external_grid;

        Set result{};
        for (auto sq = 0; sq != Grid::size; ++sq)
                if (pred(grid::ulo::Square<Grid>{sq}))
                        result.set(static_cast<std::size_t>(Board::bit_from_square(sq)));
        return result;
}

}       // namespace board
}       // namespace dctl
