#pragma once
#include <dctl/bit/detail/intrinsic.hpp>        // clznz, ctznz, ctz
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t, size_t
#include <iterator>                             // bidirectional_iterator_tag
#include <limits>                               // digits
#include <tuple>                                // tie

namespace dctl {
namespace bit {

template<class UnsignedInteger>
class ConstIterator<UnsignedInteger, 1>
:
        public boost::iterator_facade
        <
                ConstIterator<UnsignedInteger, 1>,
                std::size_t const,
                std::bidirectional_iterator_tag,
                ConstReference<UnsignedInteger, 1>,
                std::ptrdiff_t
        >
{
        static constexpr auto digits = std::numeric_limits<UnsignedInteger>::digits;
        static constexpr auto N = 1 * digits;

public:
        // constructors

        constexpr ConstIterator() = default;

        explicit constexpr ConstIterator(UnsignedInteger const* b)
        :
                block_{b},
                index_{find_first()}
        {}


        constexpr ConstIterator(UnsignedInteger const* b, std::size_t n)
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
                return static_cast<std::size_t>(bit::intrinsic::ctz(*block_));
        }

        // operator++() and operator++(int) provided by boost::iterator_facade
        constexpr auto increment()
        {
                assert(block_ != nullptr);
                assert(index_ < N);
                if (++index_ == N)
                        return;
                if (auto const mask = *block_ >> index_)
                        index_ += static_cast<std::size_t>(bit::intrinsic::ctznz(mask));
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
                if (auto const mask = *block_ << (digits - 1 - index_))
                        index_ -= static_cast<std::size_t>(bit::intrinsic::clznz(mask));
                else
                        index_ = 0;
                assert(index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<UnsignedInteger, 1> dereference() const
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

        UnsignedInteger const* block_{};
        std::size_t index_{};
};

}       // namespace bit
}       // namespace dctl
