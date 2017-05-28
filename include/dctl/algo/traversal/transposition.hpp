#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace aima {
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
}       // namespace aima
}       // namespace dctl
