#pragma once
#include <cassert>                                      // assert
#include <cstdint>                                      // CHAR_BIT
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/intrinsic.hpp>                       // clz, ctz
#include <dctl/bit/detail/storage.hpp>                  // storage

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
struct base_iterator<Block, 1>
{
        using storage = storage<Block>;
        static constexpr auto N = storage::size;

        constexpr auto find_first() noexcept
        {
                auto mask = *block_;
                return mask? bit::intrinsic::ctz(mask) : N;
        }

        constexpr void find_next() noexcept
        {
                assert(index_ < N);
                if (N <= ++index_) return;
                auto const mask = *block_ >> index_;
                index_ = mask? index_ + bit::intrinsic::ctz(mask) : N;
                assert(-1 < index_);
        }

        constexpr void find_prev() noexcept
        {
                assert(-1 < index_);
                if (--index_ <= -1) return;
                auto const mask = *block_ << (N - 1 - index_);
                index_ = mask? index_ - bit::intrinsic::clz(mask) : -1;
                assert(index_ < N);
        }

        Block const* block_;
        int index_;
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
