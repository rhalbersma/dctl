#pragma once
#include <dctl/bit/detail/base_set.hpp>         // BaseSet
#include <dctl/bit/detail/storage.hpp>          // Storage
#include <dctl/bit/iterator/iterator.hpp>       // ConstIterator
#include <dctl/bit/iterator/reference.hpp>      // ConstReference
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERKey, SinglePassRangeConcept
#include <cassert>                              // assert
#include <cstdint>                              // uintptr_t
#include <cstddef>                              // ptrdiff_t, size_t
#include <functional>                           // less
#include <initializer_list>                     // initializer_list
#include <iterator>                             // iterator_traits
#include <limits>                               // digits
#include <type_traits>                          // is_convertible
#include <utility>                              // swap

namespace dctl {
namespace bit {

template<class Block>
constexpr auto num_blocks(std::size_t N)
{
        return (N - 1) / std::numeric_limits<Block>::digits + 1;
}

template
<
        std::size_t N,
        class Key = int,
        class Compare = std::less<>
>
class Set
:
        private detail::BaseSet<Key, Compare, uintptr_t, num_blocks<uintptr_t>(N)>
{
public:
        using block_type = uintptr_t;
        static constexpr auto Nb = num_blocks<block_type>(N);
        using Base = detail::BaseSet<Key, Compare, block_type, Nb>;

        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = ConstReference<Key, block_type, Nb>;
        using const_reference = reference;
        using iterator = ConstIterator<Key, block_type, Nb>;
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

        template<class Range>
        explicit Set(Range&& rng)
        {
                BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
                insert(boost::begin(std::forward<Range>(rng)), boost::end(std::forward<Range>(rng)));
        }

        constexpr auto& operator=(std::initializer_list<value_type> ilist)
        {
                clear();
                insert(ilist);
                return *this;
        }

        auto& data()
        {
                return Base::data();
        }

        auto const& data() const
        {
                return Base::data();
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

        constexpr auto empty() const noexcept
        {
                return this->do_none();
        }

        constexpr auto size() const noexcept
        {
                return this->do_count();
        }

        static constexpr auto max_size() noexcept
        {
                return N;
        }

        // modifiers

        constexpr std::pair<iterator, bool> insert(value_type const& value)
        {
                auto const not_set = !is_mask(value);
                if (not_set)
                        set(value);
                return { { this->block_ptr(value), value }, not_set };
        }

        constexpr iterator insert(const_iterator /*hint*/, value_type const& value)
        {
                set(value);
                return { this->block_ptr(value), value };
        }

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                static_assert(std::is_convertible<decltype(*first), key_type>::value, "");
                for (auto it = first; it != last; ++it)
                        set(static_cast<key_type>(*it));
        }

        constexpr void insert(std::initializer_list<value_type> ilist)
        {
                insert(ilist.begin(), ilist.end());
        }

        template<class... Args>
        constexpr auto emplace(Args&&... args)
        {
                return insert(std::forward<Args>(args)...);
        }

        template <class... Args>
        constexpr auto emplace_hint(const_iterator position, Args&&... args)
        {
                return insert(position, std::forward<Args>(args)...);
        }

        constexpr auto erase(key_type const& key)
        {
                auto const n = count(key);
                reset(key);
                return n;
        }

        constexpr auto erase(const_iterator pos)
        {
                reset(*pos++);
                return pos;
        }

        constexpr auto erase(const_iterator first, const_iterator last)
        {
                auto it = first;
                while (it != last)
                        reset(*it++);
                return it;
        }

        constexpr auto swap(Set& other) noexcept
        {
                using std::swap;
                swap(this->data_, other.data_);
        }

        constexpr auto clear() noexcept
        {
                this->do_reset();
        }

        constexpr auto count(key_type const& key) const
        {
                return static_cast<size_type>(is_mask(key));
        }

        constexpr auto find(key_type const& key)
        {
                auto const found = is_mask(key);
                return found ? iterator{this->block_ptr(key), key} : end();
        }

        constexpr auto find(key_type const& key) const
        {
                auto const found = is_mask(key);
                return found ? const_iterator{this->block_ptr(key), key} : cend();
        }

        // relational operators

        friend auto operator==(Set const& lhs, Set const& rhs) noexcept
        {
                return lhs.do_equal(rhs);
        }

        friend auto operator!=(Set const& lhs, Set const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        friend auto operator< (Set const& lhs, Set const& rhs) noexcept
        {
                return lhs.do_colexicographical_compare(rhs);
        }

        friend auto operator>=(Set const& lhs, Set const& rhs) noexcept
        {
                return !(lhs < rhs);
        }

        friend auto operator> (Set const& lhs, Set const& rhs) noexcept
        {
                return rhs < lhs;
        }

        friend auto operator<=(Set const& lhs, Set const& rhs) noexcept
        {
                return !(rhs < lhs);
        }

        // bit access

        constexpr reference operator[](key_type n)
        {
                return { block_ref(n), n };
        }

        constexpr auto operator[](key_type n) const
        {
                return is_mask(n);
        }

        constexpr auto test(key_type n) const noexcept
        {
                return 0 <= n && n < static_cast<int>(N) && is_mask(n);
        }

        constexpr auto& reset(key_type n)
        {
                block_ref(n) &= ~mask(n);
                return *this;
        }

        constexpr auto& set(key_type n, bool value = true)
        {
                if (value)
                        block_ref(n) |= mask(n);
                else
                        block_ref(n) &= ~mask(n);
                return *this;
        }

        constexpr auto& flip(key_type n)
        {
                block_ref(n) ^= mask(n);
                return *this;
        }

        // bitwise operations

        constexpr auto& reset() noexcept
        {
                this->do_reset();
                return *this;
        }

        constexpr auto& set() noexcept
        {
                this->do_set();
                return *this;
        }

        constexpr auto& flip() noexcept
        {
                this->do_flip();
                return *this;
        }

        constexpr auto& operator&=(Set const& other) noexcept
        {
                this->do_and(other);
                return *this;
        }

        constexpr auto& operator|=(Set const& other) noexcept
        {
                this->do_or(other);
                return *this;
        }

        constexpr auto& operator^=(Set const& other) noexcept
        {
                this->do_xor(other);
                return *this;
        }

        constexpr auto& operator-=(Set const& other) noexcept
        {
                this->do_minus(other);
                return *this;
        }

        auto& operator<<=(std::size_t n)
        {
                this->do_left_shift(n);
                return *this;
        }

        auto& operator>>=(std::size_t n)
        {
                this->do_right_shift(n);
                return *this;
        }

        friend constexpr auto operator~(Set const& lhs) noexcept
        {
                auto nrv = lhs;
                nrv.flip();
                return nrv;
        }

        friend constexpr auto operator&(Set const& lhs, Set const& rhs) noexcept
        {
                auto nrv = lhs;
                nrv &= rhs;
                return nrv;
        }

        friend constexpr auto operator|(Set const& lhs, Set const& rhs) noexcept
        {
                auto nrv = lhs;
                nrv |= rhs;
                return nrv;
        }

        friend constexpr auto operator^(Set const& lhs, Set const& rhs) noexcept
        {
                auto nrv = lhs;
                nrv ^= rhs;
                return nrv;
        }

        friend constexpr auto operator-(Set const& lhs, Set const& rhs) noexcept
        {
                auto nrv = lhs;
                nrv -= rhs;
                return nrv;
        }

        friend auto operator<<(Set const& lhs, std::size_t n)
        {
                auto nrv = lhs;
                nrv <<= n;
                return nrv;
        }

        friend auto operator>>(Set const& lhs, std::size_t n)
        {
                auto nrv = lhs;
                nrv >>= n;
                return nrv;
        }

        // bitwise algorithms

        auto is_subset_of(Set const& other) const noexcept
        {
                return this->do_is_subset_of(other);
        }

        auto intersects(Set const& other) const noexcept
        {
                return this->do_intersects(other);
        }

        auto none() const noexcept
        {
                return this->do_none();
        }

        auto any() const noexcept
        {
                return this->do_any();
        }

        auto all() const noexcept
        {
                return this->do_all();
        }

        constexpr auto count() const noexcept
        {
                return this->do_count();
        }

        template<class UnaryPredicate>
        auto count_until(UnaryPredicate pred) const
        {
                return this->do_count_until(pred);
        }

private:
        constexpr auto& block_ref(key_type n)
        {
                return *(this->block_ptr(n));
        }

        constexpr auto const& block_ref(key_type n) const
        {
                return *(this->block_ptr(n));
        }

        static constexpr auto mask(key_type n)
        {
                return block_type{1} << detail::Storage<block_type>::shift_idx(n);
        }

        constexpr auto is_mask(key_type n) const
        {
                return (block_ref(n) & mask(n)) != block_type{0};
        }
};

template<std::size_t N, class Key, class Compare>
constexpr auto swap(Set<N, Key, Compare>& lhs, Set<N, Key, Compare>& rhs) noexcept
{
        lhs.swap(rhs);
        return;
}

template<std::size_t N, class Key, class Compare>
constexpr auto begin(Set<N, Key, Compare> const& s) noexcept
{
        return s.begin();
}

template<std::size_t N, class Key, class Compare>
constexpr auto end(Set<N, Key, Compare> const& s) noexcept
{
        return s.end();
}

template<std::size_t N, class Key, class Compare>
constexpr auto cbegin(Set<N, Key, Compare> const& s) noexcept
{
        return begin(s);
}

template<std::size_t N, class Key, class Compare>
constexpr auto cend(Set<N, Key, Compare> const& s) noexcept
{
        return end(s);
}

template<std::size_t N, class Key, class Compare>
/* constexpr */ auto rbegin(Set<N, Key, Compare> const& s) noexcept
{
        return s.rbegin();
}

template<std::size_t N, class Key, class Compare>
/* constexpr */ auto rend(Set<N, Key, Compare> const& s) noexcept
{
        return s.rend();
}

template<std::size_t N, class Key, class Compare>
/* constexpr */ auto crbegin(Set<N, Key, Compare> const& s) noexcept
{
        return rbegin(s);
}

template<std::size_t N, class Key, class Compare>
/* constexpr */ auto crend(Set<N, Key, Compare> const& s) noexcept
{
        return rend(s);
}

}       // namespace bit
}       // namespace dctl
