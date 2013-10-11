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

        constexpr auto find_first() noexcept
        {
                auto mask = *block_;
                return bit::ctz(mask);
        }

        constexpr void find_next() noexcept
        {
                assert(index_ < N);
                if (N <= ++index_) return;
                auto const mask = *block_ >> index_;
                index_ = mask ? index_ + bit::unchecked_ctz(mask) : N;
                assert(-1 < index_);
        }

        constexpr void find_prev() noexcept
        {
                assert(-1 < index_);
                if (--index_ <= -1) return;
                auto const mask = *block_ << (N - 1 - index_);
                index_ = mask ? index_ - bit::unchecked_clz(mask) : -1;
                assert(index_ < N);
        }

        Block const* block_;
        int index_;
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
