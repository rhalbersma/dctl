#pragma once
#include <dctl/utility/type_traits.hpp> // set_t

namespace dctl {
namespace board {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class Set = set_t<Board>, class UnaryPredicate>
constexpr auto make_set_if(UnaryPredicate pred) noexcept
{
        Set result{};
        for (auto sq = 0; sq != Board::size(); ++sq)
                if (pred(sq))
                        result.set(Board::bit_from_square(sq));
        return result;
}

}       // namespace board
}       // namespace dctl
