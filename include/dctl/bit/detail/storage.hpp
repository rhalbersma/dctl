#pragma once
#include <limits>       // digits

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
struct Storage
{
        static constexpr auto N = std::numeric_limits<Block>::digits;

        template<class T>
        static constexpr auto block_idx(T n) noexcept
        {
                return static_cast<int>(n / N);
        }

        template<class T>
        static constexpr auto shift_idx(T n) noexcept
        {
                return static_cast<int>(n % N);
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
