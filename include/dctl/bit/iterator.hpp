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

template<class T, class Block, int Nb>
class bit_iterator
:
        private detail::base_iterator<Block, Nb>
{
private:
        // typedefs

        using Base = detail::base_iterator<Block, Nb>;

public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = bit_iterator<T, Block, Nb>;
        using reference         = bit_reference<T, Block, Nb>;

        // structors

        constexpr bit_iterator() noexcept = default;

        constexpr explicit bit_iterator(Block const* b)
        :
                Base{b, this->find_first()}
        {}

        template<class U>
        constexpr bit_iterator(Block const* b, U const& value)
        :
                Base{b, int{value}}
        {
                assert(b != nullptr && value == Base::N);
                static_assert(std::is_convertible<U, int>::value, "");
        }

        // modifiers

        constexpr auto& operator++()
        {
                this->find_next();
                return *this;
        }

        constexpr auto operator++(int)
        {
                auto const old = *this;
                ++(*this);
                return old;
        }

        constexpr auto& operator--()
        {
                this->find_prev();
                return *this;
        }

        constexpr auto operator--(int)
        {
                auto const old = *this;
                --(*this);
                return old;
        }

        // queries

        constexpr reference operator*() const
        {
                assert(this->block_ != nullptr);
                return { *(this->block_), this->index_ };
        }

        // predicates

        friend constexpr bool
        operator==(bit_iterator const& lhs, bit_iterator const& rhs) noexcept
        {
                // TODO: use std::forward_as_tuple or std::tie
                //       when they become constexpr in C++14
                return lhs.block_ == rhs.block_ && lhs.index_ == rhs.index_;
        }

        friend constexpr bool
        operator!=(bit_iterator const& lhs, bit_iterator const& rhs) noexcept
        {
                return !(lhs == rhs);
        }
};

}       // namespace bit
}       // namespace dctl
