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
public:
        using Base = detail::base_iterator<Block, Nb>;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = bit_iterator<T, Block, Nb>;
        using reference         = bit_reference<T, Block, Nb>;

        // structors

        constexpr bit_iterator() noexcept = default;

        constexpr explicit bit_iterator(Block const* b) noexcept
        :
                Base{b, this->find_first()}
        {}

        template<class U>
        constexpr bit_iterator(Block const* b, U value) noexcept
        :
                Base{b, static_cast<int>(value)}
        {
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

        constexpr reference operator*()
        {
                return {*(this->block_), this->index_};
        }

        // predicates

        friend constexpr bool operator==(bit_iterator const& lhs, bit_iterator const& rhs) noexcept
        {
                return lhs.block_ == rhs.block_ && lhs.index_ == rhs.index_;
        }

        friend constexpr bool operator!=(bit_iterator const& lhs, bit_iterator const& rhs) noexcept
        {
                return !(lhs == rhs);
        }
};

}       // namespace bit
}       // namespace dctl
