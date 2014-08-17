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
        static constexpr auto block_index(T n) noexcept
        {
                return n / N;
        }

        template<class T>
        static constexpr auto bit_index(T n) noexcept
        {
                return n % N;
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
