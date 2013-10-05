#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/detail/storage.hpp>                  // storage
#include <dctl/bit/intrinsic.hpp>                       // clz, ctz

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct base_iterator
{
        static constexpr auto digits = std::numeric_limits<Block>::digits;
        static constexpr auto N = Nb * digits;

        constexpr auto find_first() noexcept
        {
                for (auto i = 0; i < Nb; ++i, ++block_) {
                        auto const mask = *block_;
                        if (mask)
                                return i * digits + intrinsic::ctz(mask);
                }
                return N;
        }

        constexpr void find_next() noexcept
        {
                assert(index_ < N);
                if (N <= ++index_) {
                        ++block_;
                        return;
                }

                auto const idx = storage<Block>::shift_idx(index_);
                if (idx == 0)
                        ++block_;
                auto const mask = *block_ >> idx;
                if (mask != 0) {
                        index_ += intrinsic::ctz(mask);
                        return;
                }
                ++block_;

                for (auto i = storage<Block>::block_idx(index_) + 1; i < Nb; ++i, ++block_) {
                        auto const mask = *block_;
                        if (mask != 0) {
                                index_ = i * digits + intrinsic::ctz(mask);
                                return;
                        }
                }
                index_ = N;
                assert(-1 < index_);
        }

        constexpr void find_prev() noexcept
        {
                assert(-1 < index_);
                if (--index_ <= -1) {
                        --block_;
                        return;
                }

                auto const idx = storage<Block>::shift_idx(index_);
                if (idx == digits - 1)
                        --block_;
                auto const mask = *block_ << (digits - 1 - idx);
                if (mask != 0) {
                        index_ -= intrinsic::clz(mask);
                        return;
                }
                --block_;

                for (auto i = storage<Block>::block_idx(index_) - 1; i >= 0; --i, --block_) {
                        auto const mask = *block_;
                        if (mask != 0) {
                                index_ = i * digits + (digits - 1 - intrinsic::clz(mask));
                                return;
                        }
                }
                index_ = -1;
                assert(index_ < N);
        }

        Block const* block_;
        int index_;
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
