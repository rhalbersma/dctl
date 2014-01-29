#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // BaseIterator
#include <dctl/bit/detail/intrinsic.hpp>                // bsfnz, bsrnz, clznz, ctznz
#include <dctl/bit/detail/storage.hpp>                  // Storage

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct BaseIterator
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = Nb * digits };

        constexpr int find_first()
        {
                assert(block_ != nullptr);
                for (auto i = 0; i < Nb; ++i) {
                        if (auto const mask = *block_)
                                return i * digits + bit::intrinsic::bsfnz(mask);
                        ++block_;
                }
                return N;
        }

        constexpr void find_next()
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ < N);
                if (N == ++index_) {
                        ++block_;
                        return;
                }

                auto const idx = Storage<Block>::shift_idx(index_);
                if (idx == 0)
                        ++block_;
                if (auto const mask = *block_ >> idx) {
                        index_ += bit::intrinsic::ctznz(mask);
                        return;
                }
                ++block_;

                for (auto i = Storage<Block>::block_idx(index_) + 1; i < Nb; ++i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits + bit::intrinsic::bsfnz(mask);
                                return;
                        }
                        ++block_;
                }
                index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        constexpr void find_prev()
        {
                assert(block_ != nullptr);
                assert(0 < index_ && index_ <= N);
                if (--index_ == 0)
                        return;

                auto const idx = Storage<Block>::shift_idx(index_);
                if (idx == digits - 1)
                        --block_;
                if (auto const mask = *block_ << (digits - 1 - idx)) {
                        index_ -= bit::intrinsic::clznz(mask);
                        return;
                }
                --block_;

                for (auto i = Storage<Block>::block_idx(index_) - 1; i >= 0; --i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits + bit::intrinsic::bsrnz(mask);
                                return;
                        }
                        --block_;
                }
                index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        Block const* block_{};
        int index_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
