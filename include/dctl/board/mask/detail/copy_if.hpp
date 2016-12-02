#pragma once
#include <dctl/utility/type_traits.hpp> // set_t

namespace dctl {
namespace board {
namespace mask {
namespace detail {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class Set = set_t<Board>, class UnaryPredicate>
constexpr auto copy_if(UnaryPredicate pred) noexcept
{
        Set result{};
        for (auto sq = 0; sq != Board::size(); ++sq)
                if (pred(sq))
                        result.insert(Board::bit_from_square(sq));
        return result;
}

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
