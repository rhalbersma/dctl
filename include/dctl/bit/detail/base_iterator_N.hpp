#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/detail/storage.hpp>                  // storage
#include <dctl/bit/intrinsic.hpp>                       // bsf, bsr, unchecked_clz, unchecked_ctz

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct base_iterator
{
        static constexpr auto digits = std::numeric_limits<Block>::digits;
        static constexpr auto N = Nb * digits;

        constexpr int find_first() noexcept
        {
                for (auto i = 0; i < Nb; ++i) {
                        auto const mask = *block_;
                        if (mask) return i * digits + bit::bsf(mask);
                        ++block_;
                }
                return N;
        }

        constexpr void find_next()
        {
                assert(0 <= index_ && index_ < N);
                if (N <= ++index_) { ++block_; return; }

                auto const idx = storage<Block>::shift_idx(index_);
                if (idx == 0) ++block_;
                auto const mask = *block_ >> idx;
                if (mask) { index_ += bit::unchecked_ctz(mask); return; }

                for (auto i = storage<Block>::block_idx(index_) + 1; i < Nb; ++i) {
                        auto const mask = *++block_;
                        if (mask) { index_ = i * digits + bit::bsf(mask); return; }
                }
                ++block_;
                index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        constexpr void find_prev()
        {
                assert(0 < index_ && index_ <= N);
                if (--index_ <= 0) return;

                auto const idx = storage<Block>::shift_idx(index_);
                if (idx == digits - 1) --block_;
                auto const mask = *block_ << (digits - 1 - idx);
                if (mask) { index_ -= bit::unchecked_clz(mask); return; }

                for (auto i = storage<Block>::block_idx(index_) - 1; i >= 0; --i) {
                        auto const mask = *--block_;
                        if (mask) { index_ = i * digits + bit::bsr(mask); return; }
                }
                index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        Block const* block_ {};
        int index_ {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
