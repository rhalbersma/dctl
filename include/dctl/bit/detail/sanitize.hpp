#pragma once
#include <dctl/bit/traits.hpp>  // all

namespace dctl {
namespace bit {
namespace detail {

// Helper class to zero out the unused high-order bits in the highest word.

template<int N>
struct Sanitize
{
        using block_type = unsigned long long;

        constexpr auto operator()(block_type& b) const noexcept
        {
                b &= ~(all<block_type> << N);
        }
};

template<>
struct Sanitize<0>
{
        using block_type = unsigned long long;

        constexpr auto operator()(block_type&) const noexcept
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
