#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // BaseIterator
#include <dctl/bit/detail/intrinsic.hpp>                // clznz, ctznz

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
struct BaseIterator<Block, 1>
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = 1 * digits };

        constexpr int find_first()
        {
                assert(block_ != nullptr);
                return bit::ctz(*block_);
        }

        constexpr void find_next()
        {
                assert(block_ != nullptr && 0 <= index_ && index_ < N);
                if (N == ++index_)
                        return;
                if (auto const mask = *block_ >> index_)
                        index_ += bit::ctznz(mask);
                else
                        index_ = N;
                assert(block_ != nullptr && 0 < index_ && index_ <= N);
        }

        constexpr void find_prev()
        {
                assert(block_ != nullptr && 0 < index_ && index_ <= N);
                if (--index_ == 0)
                        return;
                if (auto const mask = *block_ << (digits - 1 - index_))
                        index_ -= bit::clznz(mask);
                else
                        index_ = 0;
                assert(block_ != nullptr && 0 <= index_ && index_ < N);
        }

        Block const* block_{};
        int index_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
