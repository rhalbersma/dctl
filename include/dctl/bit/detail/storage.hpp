#pragma once
#include <cstdint>                      // CHAR_BIT

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct storage
{
        static constexpr auto block_size = static_cast<int>(CHAR_BIT * sizeof(Block));
        static constexpr auto max_size = Nb * block_size;

        template<class T>
        static constexpr auto block(T n) noexcept
        {
                return static_cast<int>(n / block_size);
        }

        template<class T>
        static constexpr auto index(T n) noexcept
        {
                return static_cast<int>(n % block_size);
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
