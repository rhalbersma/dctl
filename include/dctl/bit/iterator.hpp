#pragma once
#include <cstddef>                              // ptrdiff_t
#include <cstdint>                              // uint64_t, CHAR_BIT
#include <iterator>                             // iterator, bidirectional_iterator_tag
#include <dctl/bit/intrinsic.hpp>               // ctz, clz
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference

namespace dctl {
namespace bit {

template<class _>
class bit_iterator<1, _>
:
        public std::iterator<
                std::bidirectional_iterator_tag,
                int,
                std::ptrdiff_t,
                bit_iterator<1, _>,
                bit_reference<1, _>
        >
{
public:
        using storage_type = uint64_t;
        static constexpr auto N = static_cast<int>(CHAR_BIT * sizeof(storage_type));

        // structors

        constexpr bit_iterator() = default;

        constexpr explicit bit_iterator(storage_type const* s) noexcept
        :
                bit_iterator(s, (s && *s)? bit::intrinsic::ctz(*s) : N)
        {}

        constexpr bit_iterator(storage_type const* s, int i) noexcept
        :
                segment_{s},
                index_{i}
        {}

        // modifiers

        constexpr auto& operator++() noexcept
        {
                if (N <= ++index_) return *this;
                auto const mask = *segment_ >> index_;
                index_ = mask? index_ + bit::intrinsic::ctz(mask) : N;
                return *this;
        }

        constexpr auto operator++(int) noexcept
        {
                auto const old = *this;
                ++(*this);
                return old;
        }

        constexpr auto& operator--() noexcept
        {
                if (--index_ <= -1) return *this;
                auto const mask = *segment_ << (N - 1 - index_);
                index_ = mask? index_ - bit::intrinsic::clz(mask) : -1;
                return *this;
        }

        constexpr auto operator--(int) noexcept
        {
                auto const old = *this;
                --(*this);
                return old;
        }

        // views

        constexpr auto operator*() const noexcept(false) -> bit_reference<1, _>
        {
                return {*segment_, index_};
        }

        // predicates

        friend constexpr auto operator==(bit_iterator const& L, bit_iterator const& R) noexcept -> bool
        {
                return L.segment_ == R.segment_ && L.index_ == R.index_;
        }

        friend constexpr auto operator!=(bit_iterator const& L, bit_iterator const& R) noexcept -> bool
        {
                return !(L == R);
        }

private:
        // representation

        storage_type const* segment_ = nullptr;
        int index_ = N;
};

}       // namespace bit
}       // namespace dctl
