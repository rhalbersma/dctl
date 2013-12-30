#pragma once
#include <cassert>                                      // assert
#include <limits>                                       // digits
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/intrinsic.hpp>                       // clznz, ctznz

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
class base_iterator<Block, 1>
{
private:
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = 1 * digits };

public:
        constexpr int find_first() noexcept
        {
                return bit::ctz(*block_);
        }

        constexpr void find_next() noexcept
        {
                assert(0 <= index_ && index_ < N);
                if (N <= ++index_)
                        return;
                if (auto const mask = *block_ >> index_)
                        index_ += bit::ctznz(mask);
                else
                        index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        constexpr void find_prev() noexcept
        {
                assert(0 < index_ && index_ <= N);
                if (--index_ <= 0)
                        return;
                if (auto const mask = *block_ << (digits - 1 - index_))
                        index_ -= bit::clznz(mask);
                else
                        index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        Block const* block_{};
        int index_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
