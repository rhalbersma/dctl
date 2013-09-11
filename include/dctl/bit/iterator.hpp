#pragma once
#include <cstddef>                              // ptrdiff_t
#include <cstdint>                              // uint64_t
#include <iterator>                             // iterator, bidirectional_iterator_tag
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
        static constexpr auto N = static_cast<int>(sizeof(storage_type) * CHAR_BIT);
        static constexpr auto incr_mask = ~storage_type{0} << 1;
        static constexpr auto decr_mask = ~storage_type{0} >> 1;
        using self_type = bit_iterator;

        // structors

        //bit_iterator() = default;

        explicit bit_iterator(storage_type const* s)
        :
                segment_{s},
                index_{(*s)? static_cast<int>(__builtin_ctzll(*s)) : N}
        {}

        bit_iterator(storage_type const* s, int i)
        :
                segment_{s},
                index_{i}
        {}

        // modifiers

        self_type& operator++()
        {
                auto const mask = *segment_ & (incr_mask << index_);
                index_ = mask? static_cast<int>(__builtin_ctzll(mask)) : N;
                return *this;
        }

        self_type operator++(int)
        {
                auto const tmp = *this;
                ++(*this);
                return tmp;
        }

        self_type& operator--()
        {
                auto const mask = *segment_ & (decr_mask >> index_);
                index_ = mask? static_cast<int>(__builtin_clzll(mask)) : 0;
                return *this;
        }

        self_type operator--(int)
        {
                auto const tmp = *this;
                --(*this);
                return tmp;
        }

        // views

        bit_reference<1, _> operator*() const
        {
                return bit_reference<1, _>{segment_, index_};
        }

        // predicates

        friend bool operator==(bit_iterator const& L, bit_iterator const& R)
        {
                return L.segment_ == R.segment_ && L.index_ == R.index_;
        }

        friend bool operator!=(bit_iterator const& L, bit_iterator const& R)
        {
                return !(L == R);
        }

private:
        // representation

        storage_type const* segment_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
