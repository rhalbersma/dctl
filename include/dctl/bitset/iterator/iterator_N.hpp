#pragma once
#include <dctl/bitset/iterator/iterator_fwd.hpp>  // ConstIterator
#include <dctl/bitset/iterator/reference_fwd.hpp> // ConstReference
#include <dctl/bitset/intrinsic.hpp>              // bsfnz, bsrnz, clznz, ctznz
#include <dctl/bitset/limits.hpp>                 // digits, is_unsigned_integer
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag

namespace xstd {

template<class Block, int Nb, int N>
class ConstIterator
:
        public boost::iterator_facade
        <
                ConstIterator<Block, Nb, N>,
                int const,
                std::bidirectional_iterator_tag,
                ConstReference<Block, Nb, N>,
                std::ptrdiff_t
        >
{
private:
        static_assert(is_unsigned_integer<Block>, "");
        static_assert(N <= Nb * digits<Block>, "");

public:
        // constructors

        constexpr ConstIterator() = default;

        explicit constexpr ConstIterator(Block const* b)
        :
                block_{b},
                index_{find_first()}
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ <= N);
        }

        constexpr ConstIterator(Block const* b, int n)
        :
                block_{b},
                index_{n}
        {
                assert(block_ != nullptr);
                assert(index_ == N);
        }

private:
        // gateway for boost::iterator_facade to access private implementation
        friend class boost::iterator_core_access;

        constexpr auto find_first()
        {
                assert(block_ != nullptr);
                for (auto i = 0; i < Nb; ++i) {
                        if (auto const mask = *block_) {
                                assert(i * digits<Block> + intrinsic::bsfnz(mask) < N);
                                return i * digits<Block> + intrinsic::bsfnz(mask);
                        }
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
                        index_ += intrinsic::ctznz(mask);
                        assert(0 <= index_ && index_ < N);
                        return;
                }
                ++block_;

                for (auto i = index_ / digits<Block> + 1; i < Nb; ++i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<Block> + intrinsic::bsfnz(mask);
                                assert(0 <= index_ && index_ < N);
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
                if (idx == digits<Block> - 1 || index_ == N - 1)
                        --block_;
                if (auto const mask = *block_ << (digits<Block> - 1 - idx)) {
                        index_ -= intrinsic::clznz(mask);
                        assert(0 <= index_ && index_ < N);
                        return;
                }
                --block_;

                for (auto i = index_ / digits<Block> - 1; i >= 0; --i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<Block> + intrinsic::bsrnz(mask);
                                assert(0 <= index_ && index_ < N);
                                return;
                        }
                        --block_;
                }
                index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<Block, Nb, N> dereference() const
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ < N);
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

}       // namespace xstd
