#pragma once
#include <dctl/bit/detail/intrinsic.hpp>        // bsfnz, bsrnz, clznz, ctznz
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/traits.hpp>                  // digits, is_unsigned_integer
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag
#include <tuple>                                // tie

namespace dctl {
namespace bit {

template<class Block, int Nb>
class ConstIterator
:
        public boost::iterator_facade
        <
                ConstIterator<Block, Nb>,
                int const,
                std::bidirectional_iterator_tag,
                ConstReference<Block, Nb>,
                std::ptrdiff_t
        >
{
private:
        static_assert(
                is_unsigned_integer<Block>,
                "Template parameter 'T' in 'ConstIterator<T, N>' shall be of unsigned integer type."
        );

        static constexpr auto N = Nb * digits<Block>;

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
                for (auto i = 0; i < Nb; ++i) {
                        if (auto const mask = *block_)
                                return i * digits<Block> + bit::intrinsic::bsfnz(mask);
                        ++block_;
                }
                return N;
        }

        // operator++() and operator++(int) provided by boost::iterator_facade
        constexpr auto increment()
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ < N);
                if (++index_ == N) {
                        ++block_;
                        return;
                }

                auto const idx = index_ % digits<Block>;
                if (idx == 0)
                        ++block_;
                if (auto const mask = *block_ >> idx) {
                        index_ += bit::intrinsic::ctznz(mask);
                        return;
                }
                ++block_;

                for (auto i = index_ / digits<Block> + 1; i < Nb; ++i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<Block> + bit::intrinsic::bsfnz(mask);
                                return;
                        }
                        ++block_;
                }
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

                auto const idx = index_ % digits<Block>;
                if (idx == digits<Block> - 1)
                        --block_;
                if (auto const mask = *block_ << (digits<Block> - 1 - idx)) {
                        index_ -= bit::intrinsic::clznz(mask);
                        return;
                }
                --block_;

                for (auto i = index_ / digits<Block> - 1; i >= 0; --i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<Block> + bit::intrinsic::bsrnz(mask);
                                return;
                        }
                        --block_;
                }
                index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<Block, Nb> dereference() const
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
