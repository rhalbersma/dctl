#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag
#include <limits>                               // digits
#include <tuple>                                // tie
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <dctl/bit/detail/intrinsic.hpp>        // clznz, ctznz, ctz
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference

namespace dctl {
namespace bit {

template<class T, class Block>
class ConstIterator<T, Block, 1>
:
        public boost::iterator_facade
        <
                ConstIterator<T, Block, 1>,
                T const,
                std::bidirectional_iterator_tag,
                ConstReference<T, Block, 1>,
                std::ptrdiff_t
        >
{
        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = 1 * digits };

public:
        // structors

        constexpr ConstIterator() = default;

        explicit constexpr ConstIterator(Block const* b)
        :
                block_{b},
                index_{find_first()}
        {}

        template<class U>
        constexpr ConstIterator(Block const* b, U const& value)
        :
                block_{b},
                index_{value}
        {
                assert(b != nullptr);
                assert(value == N);
                static_assert(std::is_convertible<U, int>::value, "");
        }

private:
        // gateway for boost::iterator_facade to access private implementation
        friend class boost::iterator_core_access;

        constexpr int find_first()
        {
                assert(block_ != nullptr);
                return bit::intrinsic::ctz(*block_);
        }

        // operator++() and operator++(int) provided by boost::iterator_facade
        constexpr void increment()
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ < N);
                if (N == ++index_)
                        return;
                if (auto const mask = *block_ >> index_)
                        index_ += bit::intrinsic::ctznz(mask);
                else
                        index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        // operator--() and operator--(int) provided by boost::iterator_facade
        constexpr void decrement()
        {
                assert(block_ != nullptr);
                assert(0 < index_ && index_ <= N);
                if (--index_ == 0)
                        return;
                if (auto const mask = *block_ << (digits - 1 - index_))
                        index_ -= bit::intrinsic::clznz(mask);
                else
                        index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<T, Block, 1> dereference() const
        {
                assert(block_ != nullptr);
                return { *block_, index_ };
        }

        // operator== and operator!= provided by boost::iterator_facade
        constexpr auto equal(ConstIterator const& other) const noexcept
        {
                return
                        std::tie(      block_,       index_) ==
                        std::tie(other.block_, other.index_)
                ;
        }

private:
        // representation

        Block const* block_{};
        int index_{};
};

}       // namespace bit
}       // namespace dctl
