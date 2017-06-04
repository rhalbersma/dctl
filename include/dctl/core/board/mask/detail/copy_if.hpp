#pragma once
#include <dctl/util/type_traits.hpp> // set_t

namespace dctl {
namespace core {
namespace board {
namespace mask {
namespace detail {

template<class Board, class Set = set_t<Board>, class UnaryPredicate>
constexpr auto copy_if(UnaryPredicate pred) noexcept
{
        Set result{};
        for (auto sq = 0; sq < Board::size(); ++sq) {
                if (pred(sq)) {
                        result.insert(Board::bit_from_square(sq));
                }
        }
        return result;
}

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace core
}       // namespace dctl
