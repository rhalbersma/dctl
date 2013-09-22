#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference
#include <dctl/bit/detail/base_iterator.hpp>    // base_iterator

namespace dctl {
namespace bit {

template<class WordT, int Nw>
class bit_iterator
:
        private detail::base_iterator<WordT, Nw>
{
public:
        using Base = detail::base_iterator<WordT, Nw>;
        using Base::Base;

        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = bit_iterator<WordT, Nw>;
        using reference         = bit_reference<WordT, Nw>;
        using iterator_category = std::bidirectional_iterator_tag;

        // structors

        bit_iterator() = delete;

        constexpr explicit bit_iterator(WordT const* s) noexcept
        :
                Base{s, Base::do_find(s)}
        {}

        constexpr bit_iterator(WordT const* s, int i) noexcept
        :
                Base{s, i}
        {}

        // modifiers

        constexpr bit_iterator& operator++() noexcept
        {
                this->do_increment();
                return *this;
        }

        constexpr bit_iterator operator++(int) noexcept
        {
                auto const old = *this;
                ++(*this);
                return old;
        }

        constexpr bit_iterator& operator--() noexcept
        {
                this->do_decrement();
                return *this;
        }

        constexpr bit_iterator operator--(int) noexcept
        {
                auto const old = *this;
                --(*this);
                return old;
        }

        // views

        constexpr reference operator*()
        {
                return {*(this->segment_), this->index_};
        }

        // predicates

        friend constexpr bool operator==(bit_iterator const& L, bit_iterator const& R) noexcept
        {
                return L.segment_ == R.segment_ && L.index_ == R.index_;
        }

        friend constexpr bool operator!=(bit_iterator const& L, bit_iterator const& R) noexcept
        {
                return !(L == R);
        }
};

}       // namespace bit
}       // namespace dctl
