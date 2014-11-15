#pragma once
#include <dctl/type_traits.hpp>
#include <xstd/cstddef.hpp>
#include <cstddef>

namespace dctl {
namespace board {

/* NOTE: for C++11/14, constexpr predicate != lambda expression */
template<class Board, class Set = set_type_t<Board>, class UnaryPredicate>
constexpr auto make_set_if(UnaryPredicate pred) noexcept
{
        using namespace xstd::support_literals;

        Set result{};
        for (auto sq = 0; sq != Board::size(); ++sq)
                if (pred(sq))
                        result.set(Board::bit_from_square(static_cast<std::size_t>(sq)));
        return result;
}

}       // namespace board
}       // namespace dctl
