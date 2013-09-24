#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag
#include <type_traits>                          // is_convertible
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference
#include <dctl/bit/detail/base_iterator.hpp>    // base_iterator

namespace dctl {
namespace bit {

template<class T, class WordT, int Nw>
class bit_iterator
:
        private detail::base_iterator<WordT, Nw>
{
public:
        using Base = detail::base_iterator<WordT, Nw>;
        using Base::Base;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = bit_iterator<T, WordT, Nw>;
        using reference         = bit_reference<T, WordT, Nw>;

        // structors

        bit_iterator() = delete;

        constexpr explicit bit_iterator(WordT const* s) noexcept
        :
                Base{s, Base::find_first(s)}
        {}

        template<class U>
        constexpr bit_iterator(WordT const* s, U u) noexcept
        :
                Base{s, static_cast<int>(u)}
        {
                static_assert(std::is_convertible<U, int>::value, "");
        }

        // modifiers

        constexpr bit_iterator& operator++() noexcept
        {
                this->find_next();
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
                this->find_prev();
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
                static_assert(std::is_convertible<int, T>::value, "");
                return {*(this->segment_), static_cast<T>(this->index_)};
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
