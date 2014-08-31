#pragma once
#include <dctl/bit/detail/base_set.hpp>         // BaseSet
#include <dctl/bit/iterator/iterator.hpp>       // ConstIterator
#include <dctl/bit/iterator/reference.hpp>      // ConstReference
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t, size_t
#include <initializer_list>                     // initializer_list
#include <iterator>                             // iterator_traits
#include <limits>                               // digits
#include <type_traits>                          // is_convertible

namespace dctl {
namespace bit {

template<class Block>
constexpr auto num_blocks(std::size_t N)
{
        return (N - 1) / std::numeric_limits<Block>::digits + 1;
}

template<std::size_t>
class Set;

template<std::size_t N>
constexpr bool operator==(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<std::size_t N>
constexpr bool operator<(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<std::size_t N>
constexpr bool intersect(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<std::size_t N>
class Set
:
        private detail::BaseSet<std::size_t, num_blocks<std::size_t>(N)>
{
public:
        using block_type = std::size_t;
        static constexpr auto digits = std::numeric_limits<block_type>::digits;
        static constexpr auto Nb = num_blocks<block_type>(N);
        using Base = detail::BaseSet<block_type, Nb>;

        using key_type = int;
        using value_type = int;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = ConstReference<block_type, Nb>;
        using const_reference = reference;
        using iterator = ConstIterator<block_type, Nb>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // constructors

        constexpr Set() = default;

        template<class InputIt>
        constexpr Set(InputIt first, InputIt last)
        {
                insert(first, last);
        }

        constexpr Set(std::initializer_list<value_type> ilist)
        {
                insert(ilist.begin(), ilist.end());
        }

        // iterators

        constexpr auto begin() noexcept
        {
                return iterator{this->block_ptr(0)};
        }

        constexpr auto begin() const noexcept
        {
                return const_iterator{this->block_ptr(0)};
        }

        constexpr iterator end() noexcept
        {
                return { this->block_ptr(N), N };
        }

        constexpr const_iterator end() const noexcept
        {
                return { this->block_ptr(N), N };
        }

        /* constexpr */ auto rbegin() noexcept
        {
                return reverse_iterator{end()};
        }

        /* constexpr */ auto rbegin() const noexcept
        {
                return const_reverse_iterator{end()};
        }

        /* constexpr */ auto rend() noexcept
        {
                return reverse_iterator{begin()};
        }

        /* constexpr */ auto rend() const noexcept
        {
                return const_reverse_iterator{begin()};
        }

        constexpr auto cbegin() const noexcept
        {
                return begin();
        }

        constexpr auto cend() const noexcept
        {
                return end();
        }

        /* constexpr */ auto crbegin() const noexcept
        {
                return rbegin();
        }

        /* constexpr */ auto crend() const noexcept
        {
                return rend();
        }

        // capacity

        static constexpr auto max_size() noexcept
        {
                return N;
        }

        // modifiers

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                static_assert(std::is_convertible<decltype(*first), key_type>::value, "");
                for (auto it = first; it != last; ++it)
                        set(static_cast<key_type>(*it));
        }

        /* constexpr */ void swap(Set& other) noexcept
        {
                this->do_swap(other);
        }

        // element access

        constexpr reference operator[](key_type n)
        {
                return { block_ref(n), n };
        }

        constexpr auto operator[](key_type n) const
        {
                return is_mask(n);
        }

        constexpr bool test(key_type i) const noexcept
        {
                auto const n = static_cast<std::size_t>(i);
                return 0 <= n && n < N && is_mask(n);
        }

        constexpr Set& set(key_type i, bool value = true)
        {
                auto const n = static_cast<std::size_t>(i);
                if (value)
                        block_ref(n) |= mask(n);
                else
                        block_ref(n) &= ~mask(n);
                return *this;
        }

        constexpr Set& reset(key_type i)
        {
                auto const n = static_cast<std::size_t>(i);
                block_ref(n) &= ~mask(n);
                return *this;
        }

        constexpr Set& flip(key_type i)
        {
                auto const n = static_cast<std::size_t>(i);
                block_ref(n) ^= mask(n);
                return *this;
        }

        constexpr auto* data()
        {
                return this->do_data();
        }

        constexpr auto const* data() const
        {
                return this->do_data();
        }

        // comparators

        friend constexpr bool operator== <>(Set const&, Set const&) noexcept;
        friend constexpr bool operator<  <>(Set const&, Set const&) noexcept;
        friend constexpr bool intersect  <>(Set const&, Set const&) noexcept;

        constexpr bool is_subset_of(Set const& other) const noexcept
        {
                return this->do_is_subset_of(other);
        }

        constexpr bool is_superset_of(Set const& other) const noexcept
        {
                return other.is_subset_of(*this);
        }

        constexpr bool is_proper_subset_of(Set const& other) const noexcept
        {
                return this->do_is_proper_subset_of(other);
        }

        constexpr bool is_proper_superset_of(Set const& other) const noexcept
        {
                return other.is_proper_subset_of(*this);
        }

        // modifiers

        constexpr Set& set() noexcept
        {
                this->do_set();
                return *this;
        }

        constexpr Set& reset() noexcept
        {
                this->do_reset();
                return *this;
        }

        constexpr Set& flip() noexcept
        {
                this->do_flip();
                return *this;
        }

        constexpr Set& operator&=(Set const& rhs) noexcept
        {
                this->do_and(rhs);
                return *this;
        }

        constexpr Set& operator|=(Set const& rhs) noexcept
        {
                this->do_or(rhs);
                return *this;
        }

        constexpr Set& operator^=(Set const& rhs) noexcept
        {
                this->do_xor(rhs);
                return *this;
        }

        constexpr Set& operator-=(Set const& rhs) noexcept
        {
                this->do_minus(rhs);
                return *this;
        }

        constexpr Set& operator<<=(std::size_t n)
        {
                assert(n < N);
                this->do_left_shift(n);
                return *this;
        }

        constexpr Set& operator>>=(std::size_t n)
        {
                assert(n < N);
                this->do_right_shift(n);
                return *this;
        }

        // observers

        constexpr bool all() const noexcept
        {
                return this->do_all();
        }

        constexpr bool any() const noexcept
        {
                return this->do_any();
        }

        constexpr bool none() const noexcept
        {
                return this->do_none();
        }

        constexpr int count() const noexcept
        {
                return this->do_count();
        }

private:
        constexpr auto& block_ref(std::size_t n)
        {
                return *(this->block_ptr(n));
        }

        constexpr auto const& block_ref(std::size_t n) const
        {
                return *(this->block_ptr(n));
        }

        static constexpr auto mask(std::size_t n)
        {
                return one<block_type> << (n % digits);
        }

        constexpr bool is_mask(std::size_t n) const
        {
                return block_ref(n) & mask(n);
        }
};

template<std::size_t N>
constexpr bool operator==(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_equal(rhs);
}

template<std::size_t N>
constexpr bool operator<(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_colexicographical_compare(rhs);
}

template<std::size_t N>
constexpr bool operator!=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<std::size_t N>
constexpr bool operator>(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return rhs < lhs;
}

template<std::size_t N>
constexpr bool operator>=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<std::size_t N>
constexpr bool operator<=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(rhs < lhs);
}

template<std::size_t N>
/* constexpr */ void swap(Set<N>& lhs, Set<N>& rhs) noexcept
{
        lhs.swap(rhs);
}

template<std::size_t N>
constexpr bool intersect(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_intersects(rhs);
}

template<std::size_t N>
constexpr bool disjoint(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !intersect(lhs, rhs);
}

template<std::size_t N>
constexpr Set<N> operator~(Set<N> const& lhs) noexcept
{
        auto nrv(lhs);
        nrv.flip();
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator&(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv &= rhs;
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator|(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv |= rhs;
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator^(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv ^= rhs;
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator-(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv -= rhs;
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator<<(Set<N> const& lhs, std::size_t n)
{
        assert(n < N);
        auto nrv(lhs);
        nrv <<= n;
        return nrv;
}

template<std::size_t N>
constexpr Set<N> operator>>(Set<N> const& lhs, std::size_t n)
{
        assert(n < N);
        auto nrv(lhs);
        nrv >>= n;
        return nrv;
}

template<std::size_t N>
constexpr auto begin(Set<N> const& s) noexcept
{
        return s.begin();
}

template<std::size_t N>
constexpr auto end(Set<N> const& s) noexcept
{
        return s.end();
}

template<std::size_t N>
constexpr auto cbegin(Set<N> const& s) noexcept
{
        return begin(s);
}

template<std::size_t N>
constexpr auto cend(Set<N> const& s) noexcept
{
        return end(s);
}

template<std::size_t N>
/* constexpr */ auto rbegin(Set<N> const& s) noexcept
{
        return s.rbegin();
}

template<std::size_t N>
/* constexpr */ auto rend(Set<N> const& s) noexcept
{
        return s.rend();
}

template<std::size_t N>
/* constexpr */ auto crbegin(Set<N> const& s) noexcept
{
        return rbegin(s);
}

template<std::size_t N>
/* constexpr */ auto crend(Set<N> const& s) noexcept
{
        return rend(s);
}

}       // namespace bit
}       // namespace dctl
