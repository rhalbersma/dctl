#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>      // size_t

namespace dctl::algo {
namespace traversal {

class Transposition
{
        std::size_t nodes_ : 59;        // 5.8e17 nodes
        std::size_t depth_ :  5;        // 32 ply

public:
        constexpr Transposition() noexcept
        :
                nodes_{0},
                depth_{0}
        {}

        constexpr Transposition(std::size_t const n, std::size_t const d) noexcept
        :
                nodes_{n & ((1ULL << 59) - 1)},
                depth_{d & ((1ULL <<  5) - 1)}
        {}

        constexpr auto nodes() const noexcept
        {
                return nodes_;
        }

        constexpr auto depth() const noexcept
        {
                return depth_;
        }
};

}       // namespace traversal
}       // namespace dctl::algo
