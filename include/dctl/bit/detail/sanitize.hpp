#pragma once
#include <dctl/bit/traits.hpp>  // all, digits

namespace dctl {
namespace bit {
namespace detail {

// Helper classes to zero out the unused high-order bits in the highest word.

template<int N, bool = N < digits<unsigned long long>>
struct Sanitize;

template<int N>
struct Sanitize<N, true>
{
        using block_type = unsigned long long;
        constexpr auto operator()(block_type b) const noexcept
        {
                return b & ~(all<block_type> << N);
        }
};

template<int N>
struct Sanitize<N, false>
{
        using block_type = unsigned long long;
        constexpr auto operator()(block_type b) const noexcept
        {
                return b;
        }
};

template<int N>
struct SanitizeAssign
{
        using block_type = unsigned long long;
        constexpr auto operator()(block_type& b) const noexcept
        {
                b &= ~(all<block_type> << N);
        }
};

template<>
struct SanitizeAssign<0>
{
        using block_type = unsigned long long;
        constexpr auto operator()(block_type& /* b */) const noexcept
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
