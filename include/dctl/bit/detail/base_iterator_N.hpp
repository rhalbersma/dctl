#pragma once
#include <cassert>                                      // assert
#include <cstdint>                                      // CHAR_BIT
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/intrinsic.hpp>                       // clz, ctz
#include <dctl/bit/detail/storage.hpp>                  // storage

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct base_iterator
{
        using storage = detail::storage<Block>;
        static constexpr auto N = Nb * storage::size;

        constexpr auto find_first() noexcept
        {
                for (auto i = 0; i < Nb; ++i, ++block_) {
                        auto const mask = *block_;
                        if (mask)
                                return i * storage::size + bit::intrinsic::ctz(mask);
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

                auto const idx = storage::index(index_);
                if (idx == 0) {
                        index_ = storage::block(index_) * storage::size;
                        ++block_;
                }
                auto const mask = *block_ >> idx;
                if (mask) {
                        index_ += bit::intrinsic::ctz(mask);
                        return;
                }
                ++block_;

                for (auto i = storage::block(index_) + 1; i < Nb; ++i, ++block_) {
                        auto const mask = *block_;
                        if (mask != 0) {
                                index_ = i * storage::size + bit::intrinsic::ctz(mask);
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

                auto const idx = storage::index(index_);
                if (idx == storage::size - 1)
                        --block_;
                auto const mask = *block_ << (storage::size - 1 - idx);
                if (mask) {
                        index_ -= bit::intrinsic::clz(mask);
                        return;
                }
                --block_;

                for (auto i = storage::block(index_) - 1; i >= 0; --i, --block_) {
                        auto const mask = *block_;
                        if (mask != 0) {
                                index_ = i * storage::size + (storage::size - 1 - bit::intrinsic::clz(mask));
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
