#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace walk {

class Transposition
{
        std::size_t nodes_ : 59;        // 5.8e17 nodes
        int         depth_ :  5;        // 32 ply

public:
        constexpr Transposition(std::size_t n, int d) noexcept
        :
                nodes_{n},
                depth_{d}
        {}

        constexpr Transposition() noexcept
        :
                nodes_{0},
                depth_{0}
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

}       // namespace walk
}       // namespace dctl
