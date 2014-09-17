#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/intrinsic.hpp>               // bsfnz, bsrnz, clznz, ctznz
#include <dctl/bit/traits.hpp>                  // digits, is_unsigned_integer
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag

namespace dctl {
namespace bit {

template<int N, int Nb>
class ConstIterator
:
        public boost::iterator_facade
        <
                ConstIterator<N, Nb>,
                int const,
                std::bidirectional_iterator_tag,
                ConstReference<N, Nb>,
                std::ptrdiff_t
        >
{
private:
        using block_type = unsigned long long;
        static_assert(N <= Nb * digits<block_type>, "");

public:
        // constructors

        constexpr ConstIterator() = default;

        explicit constexpr ConstIterator(block_type const* b)
        :
                block_{b},
                index_{find_first()}
        {
                assert(block_ != nullptr);
                assert(0 <= index_ && index_ <= N);
        }

        constexpr ConstIterator(block_type const* b, int n)
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
                                assert(i * digits<block_type> + bit::intrinsic::bsfnz(mask) < N);
                                return i * digits<block_type> + bit::intrinsic::bsfnz(mask);
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

                auto const idx = index_ % digits<block_type>;
                if (idx == 0)
                        ++block_;
                if (auto const mask = *block_ >> idx) {
                        index_ += bit::intrinsic::ctznz(mask);
                        assert(0 <= index_ && index_ < N);
                        return;
                }
                ++block_;

                for (auto i = index_ / digits<block_type> + 1; i < Nb; ++i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<block_type> + bit::intrinsic::bsfnz(mask);
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

                auto const idx = index_ % digits<block_type>;
                if (idx == digits<block_type> - 1)
                        --block_;
                if (auto const mask = *block_ << (digits<block_type> - 1 - idx)) {
                        index_ -= bit::intrinsic::clznz(mask);
                        assert(0 <= index_ && index_ < N);
                        return;
                }
                --block_;

                for (auto i = index_ / digits<block_type> - 1; i >= 0; --i) {
                        if (auto const mask = *block_) {
                                index_ = i * digits<block_type> + bit::intrinsic::bsrnz(mask);
                                assert(0 <= index_ && index_ < N);
                                return;
                        }
                        --block_;
                }
                index_ = 0;
                assert(0 <= index_ && index_ < N);
        }

        // operator* provided by boost::iterator_facade
        constexpr ConstReference<N, Nb> dereference() const
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

        block_type const* block_{};
        int index_{};
};

}       // namespace bit
}       // namespace dctl
