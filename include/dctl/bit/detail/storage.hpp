#pragma once
#include <cstdint>                      // CHAR_BIT

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
struct storage
{
        static constexpr auto size = static_cast<int>(CHAR_BIT * sizeof(Block));

        template<class T>
        static constexpr auto block(T n) noexcept
        {
                return static_cast<int>(n / size);
        }

        template<class T>
        static constexpr auto index(T n) noexcept
        {
                return static_cast<int>(n % size);
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
