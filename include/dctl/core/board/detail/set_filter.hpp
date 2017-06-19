#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/util/type_traits.hpp>    // set_t

namespace dctl::core {
namespace detail {

template<class Board, class UnaryPredicate>
constexpr auto set_filter(UnaryPredicate pred) noexcept
{
        auto result = set_t<Board>{};
        for (auto sq = 0; sq < Board::size(); ++sq) {
                if (pred(sq)) {
                        result.insert(Board::bit_from_square(sq));
                }
        }
        return result;
}

}       // namespace detail
}       // namespace dctl::core
