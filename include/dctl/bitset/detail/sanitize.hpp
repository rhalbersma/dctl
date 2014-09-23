#pragma once
#include <dctl/bitset/limits.hpp>       // digits
#include <dctl/bitset/masks.hpp>        // all

namespace xstd {
namespace detail {

// Helper classes to zero out the unused high-order bits in the highest word.

template<int N, bool = N < digits<unsigned long long>>
struct Sanitize;

template<int N>
struct Sanitize<N, true>
{
        template<class Block>
        constexpr auto operator()(Block b) const noexcept
        {
                return b & ~(masks::all<Block> << N);
        }
};

template<int N>
struct Sanitize<N, false>
{
        template<class Block>
        constexpr auto operator()(Block b) const noexcept
        {
                return b;
        }
};

template<int N>
struct SanitizeAssign
{
        template<class Block>
        constexpr auto operator()(Block& b) const noexcept
        {
                b &= ~(masks::all<Block> << N);
        }
};

template<>
struct SanitizeAssign<0>
{
        template<class Block>
        constexpr auto operator()(Block& /* b */) const noexcept
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace xstd
