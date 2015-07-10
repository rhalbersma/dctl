#pragma once
#include <dctl/set_type.hpp>    // set_type

namespace dctl {
namespace board {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class Set = get_set_type<Board>, class UnaryPredicate>
constexpr auto make_set_if(UnaryPredicate pred) noexcept
{
        using namespace xstd::support_literals;

        Set result;
        for (auto sq = 0_z; sq != Board::size(); ++sq)
                if (pred(sq))
                        result.set(Board::bit_from_square(sq));
        return result;
}

}       // namespace board
}       // namespace dctl
