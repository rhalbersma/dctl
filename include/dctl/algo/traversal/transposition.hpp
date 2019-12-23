#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>      // uint64_t

namespace dctl::algo {
namespace traversal {

class Transposition
{
        uint64_t nodes_ : 59;        // 5.8e17 nodes
        uint64_t depth_ :  5;        // 32 ply

public:
        constexpr Transposition() noexcept
        :
                nodes_{0},
                depth_{0}
        {}

        constexpr Transposition(uint64_t const n, uint64_t const d) noexcept
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
