#pragma once
#include <dctl/bit/detail/intrinsic.hpp>        // clznz, ctznz, ctz
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/traits.hpp>                  // digits, is_unsigned_integer
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag

namespace dctl {
namespace bit {

template<class Block>
class ConstIterator<Block, 1>
:
        public boost::iterator_facade
        <
                ConstIterator<Block, 1>,
                int const,
                std::bidirectional_iterator_tag,
                ConstReference<Block, 1>,
                std::ptrdiff_t
        >
{
        static_assert(is_unsigned_integer<Block>, "");
        static constexpr auto N = 1 * digits<Block>;

public:
        // constructors

        constexpr ConstIterator() = default;

        explicit constexpr ConstIterator(Block const* b)
        :
                block_{b},
                index_{find_first()}
        {}

        constexpr ConstIterator(Block const* b, int n)
        :
                block_{b},
                index_{n}
        {
                assert(b != nullptr);
                assert(n == N);
        }

private:
        // gateway for boost::iterator_facade to access private implementation
        friend class boost::iterator_core_access;

        constexpr auto find_first()
        {
                assert(block_ != nullptr);
                return bit::intrinsic::ctz(*block_);
        }

        // operator++() and operator++(int) provided by boost::iterator_facade
        constexpr auto increment()
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ < N);
                if (++index_ == N)
                        return;
                if (auto const mask = *block_ >> index_)
                        index_ += bit::intrinsic::ctznz(mask);
                else
                        index_ = N;
                assert(0 < index_ && index_ <= N);
        }

        // operator--() and operator--(int) provided by boost::iterator_facade
        constexpr auto decrement()
        {
                assert(block_ != nullptr);
                assert(0 < index_ && index_ <= N);
                if (--index_ == 0)
                        return;
                if (auto const mask = *block_ << (digits<Block> - 1 - index_))
                        index_ -= bit::intrinsic::clznz(mask);
                else
                        index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<Block, 1> dereference() const
        {
                assert(block_ != nullptr);
                return { *block_, index_ };
        }

        // operator== and operator!= provided by boost::iterator_facade
        constexpr auto equal(ConstIterator const& other) const noexcept
        {
                return block_ == other.block_ && index_ == other.index_;
        }

private:
        // representation

        Block const* block_{};
        int index_{};
};

}       // namespace bit
}       // namespace dctl
