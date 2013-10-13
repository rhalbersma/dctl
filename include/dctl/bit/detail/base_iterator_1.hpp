#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/intrinsic.hpp>                       // unchecked_clz, unchecked_ctz

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
struct base_iterator<Block, 1>
{
        static constexpr auto N = std::numeric_limits<Block>::digits;

        constexpr int find_first() noexcept
        {
                return bit::ctz(*block_);
        }

        constexpr void find_next()
        {
                assert(0 <= index_ && index_ < N);
                if (N <= ++index_) return;
                auto const mask = *block_ >> index_;
                if (mask)
                        index_ += bit::unchecked_ctz(mask);
                else
                        index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        constexpr void find_prev()
        {
                assert(0 < index_ && index_ <= N);
                if (--index_ <= 0) return;
                auto const mask = *block_ << (N - 1 - index_);
                if (mask)
                        index_ -= bit::unchecked_clz(mask);
                else
                        index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        Block const* block_ {};
        int index_ {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
