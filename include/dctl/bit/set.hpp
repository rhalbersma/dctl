#pragma once
#include <dctl/bit/detail/base_set.hpp>         // BaseSet
#include <dctl/bit/iterator/iterator.hpp>       // ConstIterator
#include <dctl/bit/iterator/reference.hpp>      // ConstReference
#include <dctl/bit/traits.hpp>                  // one, digits
#include <cassert>                              // assert
#include <initializer_list>                     // initializer_list
#include <iterator>                             // reverse_iterator

namespace dctl {
namespace bit {

template<class Block>
constexpr auto num_blocks(int N)
{
        return (N - 1) / digits<Block> + 1;
}

template<int>
class Set;

template<int N>
constexpr bool operator==(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<int N>
constexpr bool operator<(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<int N>
constexpr bool intersect(Set<N> const& lhs, Set<N> const& rhs) noexcept;

template<int N>
class Set
:
        private detail::BaseSet<unsigned long long, num_blocks<unsigned long long>(N)>
{
private:
        using block_type = unsigned long long;
        static constexpr auto Nb = num_blocks<block_type>(N);

public:
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

        constexpr Set(std::initializer_list<int> ilist)
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

        static constexpr int size() noexcept
        {
                return N;
        }

        // modifiers

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        set(*it);
        }

        /* constexpr */ void swap(Set& other) noexcept
        {
                this->do_swap(other);
        }

        // element access

        constexpr reference operator[](int n)
        {
                assert(0 <= n && n < N);
                return { block_ref(n), n };
        }

        constexpr auto operator[](int n) const
        {
                assert(0 <= n && n < N);
                return is_mask(n);
        }

        constexpr bool test(int n) const noexcept
        {
                return 0 <= n && n < N && is_mask(n);
        }

        constexpr Set& set(int n)
        {
                assert(0 <= n && n < N);
                block_ref(n) |= mask(n);
                assert(is_mask(n));
                return *this;
        }

        constexpr Set& reset(int n)
        {
                assert(0 <= n && n < N);
                block_ref(n) &= ~mask(n);
                assert(!is_mask(n));
                return *this;
        }

        constexpr Set& flip(int n)
        {
                assert(0 <= n && n < N);
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
                assert(all());
                return *this;
        }

        constexpr Set& reset() noexcept
        {
                this->do_reset();
                assert(none());
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

        constexpr Set& operator<<=(int n)
        {
                assert(0 <= n && n < N);
                this->do_left_shift(n);
                return *this;
        }

        constexpr Set& operator>>=(int n)
        {
                assert(0 <= n && n < N);
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
        constexpr auto& block_ref(int n)
        {
                assert(0 <= n && n < N);
                return *(this->block_ptr(n));
        }

        constexpr auto const& block_ref(int n) const
        {
                assert(0 <= n && n < N);
                return *(this->block_ptr(n));
        }

        static constexpr auto mask(int n)
        {
                return one<block_type> << (n % digits<block_type>);
        }

        constexpr bool is_mask(int n) const
        {
                assert(0 <= n && n < N);
                return block_ref(n) & mask(n);
        }
};

template<int N>
constexpr bool operator==(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_equal(rhs);
}

template<int N>
constexpr bool operator<(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_colexicographical_compare(rhs);
}

template<int N>
constexpr bool operator!=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<int N>
constexpr bool operator>(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return rhs < lhs;
}

template<int N>
constexpr bool operator>=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<int N>
constexpr bool operator<=(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !(rhs < lhs);
}

template<int N>
/* constexpr */ void swap(Set<N>& lhs, Set<N>& rhs) noexcept
{
        lhs.swap(rhs);
}

template<int N>
constexpr bool intersect(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return lhs.do_intersects(rhs);
}

template<int N>
constexpr bool disjoint(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        return !intersect(lhs, rhs);
}

template<int N>
constexpr Set<N> operator~(Set<N> const& lhs) noexcept
{
        auto nrv(lhs);
        nrv.flip();
        return nrv;
}

template<int N>
constexpr Set<N> operator&(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv &= rhs;
        return nrv;
}

template<int N>
constexpr Set<N> operator|(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv |= rhs;
        return nrv;
}

template<int N>
constexpr Set<N> operator^(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv ^= rhs;
        return nrv;
}

template<int N>
constexpr Set<N> operator-(Set<N> const& lhs, Set<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv -= rhs;
        return nrv;
}

template<int N>
constexpr Set<N> operator<<(Set<N> const& lhs, int n)
{
        assert(0 <= n && n < N);
        auto nrv(lhs);
        nrv <<= n;
        return nrv;
}

template<int N>
constexpr Set<N> operator>>(Set<N> const& lhs, int n)
{
        assert(0 <= n && n < N);
        auto nrv(lhs);
        nrv >>= n;
        return nrv;
}

template<int N>
constexpr auto begin(Set<N> const& s) noexcept
{
        return s.begin();
}

template<int N>
constexpr auto end(Set<N> const& s) noexcept
{
        return s.end();
}

template<int N>
constexpr auto cbegin(Set<N> const& s) noexcept
{
        return begin(s);
}

template<int N>
constexpr auto cend(Set<N> const& s) noexcept
{
        return end(s);
}

template<int N>
/* constexpr */ auto rbegin(Set<N> const& s) noexcept
{
        return s.rbegin();
}

template<int N>
/* constexpr */ auto rend(Set<N> const& s) noexcept
{
        return s.rend();
}

template<int N>
/* constexpr */ auto crbegin(Set<N> const& s) noexcept
{
        return rbegin(s);
}

template<int N>
/* constexpr */ auto crend(Set<N> const& s) noexcept
{
        return rend(s);
}

}       // namespace bit
}       // namespace dctl
