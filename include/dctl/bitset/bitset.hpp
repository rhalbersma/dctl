#pragma once
#include <dctl/bitset/detail/base_bitset.hpp>     // base_bitset
#include <dctl/bitset/detail/sanitize.hpp>
#include <dctl/bitset/iterator/iterator.hpp>      // ConstIterator
#include <dctl/bitset/iterator/reference.hpp>     // ConstReference
#include <dctl/bitset/limits.hpp>                 // digits
#include <dctl/bitset/masks.hpp>                  // one
#include <cassert>                              // assert
#include <initializer_list>                     // initializer_list
#include <iterator>                             // reverse_iterator

namespace xstd {

template<class Block>
constexpr auto num_blocks(int N)
{
        return (N - 1 + digits<Block>) / digits<Block>;
}

template<int>
class bitset;

template<int N>
constexpr bool operator==(bitset<N> const& lhs, bitset<N> const& rhs) noexcept;

template<int N>
constexpr bool operator<(bitset<N> const& lhs, bitset<N> const& rhs) noexcept;

template<int N>
constexpr bool intersect(bitset<N> const& lhs, bitset<N> const& rhs) noexcept;

template<int N>
class bitset
:
        private detail::base_bitset<unsigned long long, num_blocks<unsigned long long>(N)>
{
        using block_type = unsigned long long;
        static constexpr auto Nb = num_blocks<block_type>(N);
        using Base = detail::base_bitset<unsigned long long, num_blocks<unsigned long long>(N)>;

public:
        using reference = ConstReference<block_type, Nb, N>;
        using const_reference = reference;
        using iterator = ConstIterator<block_type, Nb, N>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // constructors

        constexpr bitset() = default;

        /* implicit */ constexpr bitset(unsigned long long value) noexcept
        :
                Base{detail::Sanitize<N>{}(value)}
        {}

        template<class ForwardIterator>
        constexpr bitset(ForwardIterator first, ForwardIterator last)
        {
                for (auto it = first; it != last; ++it)
                        set(*it);
        }

        constexpr bitset(std::initializer_list<int> ilist)
        :
                bitset(ilist.begin(), ilist.end())
        {}

        // iterators

        constexpr auto begin() noexcept
        {
                return iterator{this->block_begin()};
        }

        constexpr auto begin() const noexcept
        {
                return const_iterator{this->block_begin()};
        }

        constexpr iterator end() noexcept
        {
                return { this->block_end(), N };
        }

        constexpr const_iterator end() const noexcept
        {
                return { this->block_end(), N };
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

        /* constexpr */ void swap(bitset& other) noexcept
        {
                this->do_swap(other);
        }

        // element access

        [[deprecated]] constexpr reference operator[](int n)
        {
                assert(0 <= n && n < N);
                return { this->block_ref(n), n };
        }

        [[deprecated]] constexpr bool operator[](int n) const
        {
                assert(0 <= n && n < N);
                return test(n);
        }

        constexpr bool test(int n) const
        {
                assert(0 <= n && n < N);
                return this->block_ref(n) & mask(n);
        }

        [[deprecated]] constexpr bitset& set(int n, bool value)
        {
                return value ? set(n) : reset(n);
        }

        constexpr bitset& set(int n)
        {
                assert(0 <= n && n < N);
                this->block_ref(n) |= mask(n);
                assert(test(n));
                return *this;
        }

        constexpr bitset& reset(int n)
        {
                assert(0 <= n && n < N);
                this->block_ref(n) &= ~mask(n);
                assert(!test(n));
                return *this;
        }

        constexpr bitset& flip(int n)
        {
                assert(0 <= n && n < N);
                this->block_ref(n) ^= mask(n);
                return *this;
        }

        constexpr auto* data() noexcept
        {
                return this->block_begin();
        }

        constexpr auto const* data() const noexcept
        {
                return this->block_begin();
        }

        // comparators

        friend constexpr bool operator== <>(bitset const&, bitset const&) noexcept;
        friend constexpr bool operator<  <>(bitset const&, bitset const&) noexcept;
        friend constexpr bool intersect  <>(bitset const&, bitset const&) noexcept;

        constexpr bool is_subset_of(bitset const& other) const noexcept
        {
                return this->do_is_subset_of(other);
        }

        constexpr bool is_superset_of(bitset const& other) const noexcept
        {
                return other.is_subset_of(*this);
        }

        constexpr bool is_proper_subset_of(bitset const& other) const noexcept
        {
                return this->do_is_proper_subset_of(other);
        }

        constexpr bool is_proper_superset_of(bitset const& other) const noexcept
        {
                return other.is_proper_subset_of(*this);
        }

        // modifiers

        constexpr bitset& set() noexcept
        {
                this->do_set();
                sanitize();
                assert(all());
                return *this;
        }

        constexpr bitset& reset() noexcept
        {
                this->do_reset();
                assert(none());
                return *this;
        }

        constexpr bitset& flip() noexcept
        {
                this->do_flip();
                sanitize();
                return *this;
        }

        constexpr bitset& operator&=(bitset const& rhs) noexcept
        {
                this->do_and(rhs);
                return *this;
        }

        constexpr bitset& operator|=(bitset const& rhs) noexcept
        {
                this->do_or(rhs);
                return *this;
        }

        constexpr bitset& operator^=(bitset const& rhs) noexcept
        {
                this->do_xor(rhs);
                return *this;
        }

        constexpr bitset& operator-=(bitset const& rhs) noexcept
        {
                this->do_minus(rhs);
                return *this;
        }

        constexpr bitset& operator<<=(int n)
        {
                assert(0 <= n && n < N);
                this->do_left_shift(n);
                sanitize();
                return *this;
        }

        constexpr bitset& operator>>=(int n)
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
        constexpr auto sanitize() noexcept
        {
                detail::SanitizeAssign<N % digits<block_type>>{}(this->block_back());
        }

        constexpr auto mask(int n) const noexcept
        {
                return masks::one<block_type> << (n % digits<block_type>);
        }
};

template<int N>
constexpr bool operator==(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return lhs.do_equal(rhs);
}

template<int N>
constexpr bool operator<(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return lhs.do_colexicographical_compare(rhs);
}

template<int N>
constexpr bool operator!=(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<int N>
constexpr bool operator>(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return rhs < lhs;
}

template<int N>
constexpr bool operator>=(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<int N>
constexpr bool operator<=(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return !(rhs < lhs);
}

template<int N>
/* constexpr */ void swap(bitset<N>& lhs, bitset<N>& rhs) noexcept
{
        lhs.swap(rhs);
}

template<int N>
constexpr bool intersect(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return lhs.do_intersects(rhs);
}

template<int N>
constexpr bool disjoint(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        return !intersect(lhs, rhs);
}

template<int N>
constexpr bitset<N> operator~(bitset<N> const& lhs) noexcept
{
        auto nrv(lhs);
        nrv.flip();
        return nrv;
}

template<int N>
constexpr bitset<N> operator&(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv &= rhs;
        return nrv;
}

template<int N>
constexpr bitset<N> operator|(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv |= rhs;
        return nrv;
}

template<int N>
constexpr bitset<N> operator^(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv ^= rhs;
        return nrv;
}

template<int N>
constexpr bitset<N> operator-(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        auto nrv(lhs);
        nrv -= rhs;
        return nrv;
}

template<int N>
constexpr bitset<N> operator<<(bitset<N> const& lhs, int n)
{
        assert(0 <= n && n < N);
        auto nrv(lhs);
        nrv <<= n;
        return nrv;
}

template<int N>
constexpr bitset<N> operator>>(bitset<N> const& lhs, int n)
{
        assert(0 <= n && n < N);
        auto nrv(lhs);
        nrv >>= n;
        return nrv;
}

template<int N>
constexpr auto begin(bitset<N> const& s) noexcept
{
        return s.begin();
}

template<int N>
constexpr auto end(bitset<N> const& s) noexcept
{
        return s.end();
}

template<int N>
constexpr auto cbegin(bitset<N> const& s) noexcept
{
        return begin(s);
}

template<int N>
constexpr auto cend(bitset<N> const& s) noexcept
{
        return end(s);
}

template<int N>
/* constexpr */ auto rbegin(bitset<N> const& s) noexcept
{
        return s.rbegin();
}

template<int N>
/* constexpr */ auto rend(bitset<N> const& s) noexcept
{
        return s.rend();
}

template<int N>
/* constexpr */ auto crbegin(bitset<N> const& s) noexcept
{
        return rbegin(s);
}

template<int N>
/* constexpr */ auto crend(bitset<N> const& s) noexcept
{
        return rend(s);
}

}       // namespace xstd
