#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // bidirectional_iterator_tag
#include <type_traits>                          // is_convertible
#include <dctl/bit/detail/base_iterator.hpp>    // BaseIterator
#include <dctl/bit/iterator_fwd.hpp>            // Iterator
#include <dctl/bit/reference_fwd.hpp>           // Reference

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class Iterator
:
        private detail::BaseIterator<Block, Nb>
{
private:
        // typedefs

        using Base = detail::BaseIterator<Block, Nb>;

public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Iterator<T, Block, Nb>;
        using reference         = Reference<T, Block, Nb>;

        // structors

        constexpr Iterator() noexcept = default;

        constexpr explicit Iterator(Block const* b)
        :
                Base{b, this->find_first()}
        {}

        template<class U>
        constexpr Iterator(Block const* b, U const& value)
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
        operator==(Iterator const& lhs, Iterator const& rhs) noexcept
        {
                // TODO: use std::forward_as_tuple or std::tie
                //       when they become constexpr in C++14
                return
                        lhs.block_ == rhs.block_ &&
                        lhs.index_ == rhs.index_
                ;
        }

        friend constexpr bool
        operator!=(Iterator const& lhs, Iterator const& rhs) noexcept
        {
                return !(lhs == rhs);
        }
};

}       // namespace bit
}       // namespace dctl
