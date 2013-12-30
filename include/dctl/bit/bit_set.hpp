#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t, size_t
#include <cstdint>                              // uint64_t
#include <initializer_list>                     // initializer_list
#include <iterator>                             // distance, iterator_traits
#include <limits>                               // digits
#include <type_traits>                          // is_convertible
#include <utility>                              // swap
#include <dctl/bit/iterator.hpp>                // bit_iterator
#include <dctl/bit/reference.hpp>               // bit_reference
#include <dctl/bit/detail/base_set.hpp>         // base_set
#include <dctl/bit/detail/storage.hpp>          // storage

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class bit_set
:
        private detail::base_set<T, Block, Nb>
{
public:
        using Base = detail::base_set<T, Block, Nb>;
        enum { N = Nb * std::numeric_limits<Block>::digits };

        using key_type = T;
        using value_type = T;
        using size_type = int;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<T, Block, Nb>;
        using const_reference = reference;
        using iterator = bit_iterator<T, Block, Nb>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // structors

        constexpr bit_set() = default;

        template<class InputIt>
        constexpr bit_set(InputIt first, InputIt last)
        {
                insert(first, last);
        }

        constexpr bit_set(std::initializer_list<value_type> ilist)
        :
                bit_set{ilist.begin(), ilist.end()}
        {}

        constexpr auto& operator=(std::initializer_list<value_type> ilist)
        {
                clear();
                insert(ilist);
                return *this;
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

        constexpr auto max_size() const noexcept
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
                static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, key_type>::value, "");
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

        constexpr void swap(bit_set& other) noexcept
        {
                using std::swap;
                swap(this->data_, other.data_);
        }

        constexpr void clear() noexcept
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

        friend constexpr bool operator==(bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return Base::do_equal(lhs, rhs);
        }

        friend constexpr bool operator!=(bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        friend constexpr bool operator< (bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return Base::do_lexicograhical_compare(lhs, rhs);
        }

        friend constexpr bool operator>=(bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return !(lhs < rhs);
        }

        friend constexpr bool operator> (bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return rhs < lhs;
        }

        friend constexpr bool operator<=(bit_set const& lhs, bit_set const& rhs) noexcept
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
                return 0 <= n && n < N && is_mask(n);
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

        constexpr auto& operator&=(bit_set const& other) noexcept
        {
                this->do_and(other);
                return *this;
        }

        constexpr auto& operator|=(bit_set const& other) noexcept
        {
                this->do_or(other);
                return *this;
        }

        constexpr auto& operator^=(bit_set const& other) noexcept
        {
                this->do_xor(other);
                return *this;
        }

        constexpr auto& operator<<=(int n)
        {
                this->do_left_shift(n);
                return *this;
        }

        constexpr auto& operator>>=(int n)
        {
                this->do_right_shift(n);
                return *this;
        }

        friend constexpr auto operator~(bit_set const& lhs) noexcept -> bit_set
        {
                bit_set nrv{lhs};
                nrv.flip();
                return nrv;
        }

        friend constexpr auto operator&(bit_set const& lhs, bit_set const& rhs) noexcept -> bit_set
        {
                bit_set nrv{lhs};
                nrv &= rhs;
                return nrv;
        }

        friend constexpr auto operator|(bit_set const& lhs, bit_set const& rhs) noexcept -> bit_set
        {
                bit_set nrv{lhs};
                nrv |= rhs;
                return nrv;
        }

        friend constexpr auto operator^(bit_set const& lhs, bit_set const& rhs) noexcept -> bit_set
        {
                bit_set nrv{lhs};
                nrv ^= rhs;
                return nrv;
        }

        friend constexpr auto operator<<(bit_set const& lhs, int n) -> bit_set
        {
                bit_set nrv{lhs};
                nrv <<= n;
                return nrv;
        }

        friend constexpr auto operator>>(bit_set const& lhs, int n) -> bit_set
        {
                bit_set nrv{lhs};
                nrv >>= n;
                return nrv;
        }

        // bitwise algorithms

        constexpr auto includes(bit_set const& other) const noexcept
        {
                return this->do_includes(other);
        }

        constexpr auto intersects(bit_set const& other) const noexcept
        {
                return this->do_intersects(other);
        }

        constexpr auto none() const noexcept
        {
                return this->do_none();
        }

        constexpr auto any() const noexcept
        {
                return this->do_any();
        }

        constexpr auto all() const noexcept
        {
                return this->do_all();
        }

        constexpr auto is_count_equal_to(int n) const noexcept
        {
                return this->do_is_count_equal_to(n);
        }

        constexpr auto is_count_less(int n) const noexcept
        {
                return this->do_is_count_less(n);
        }

        constexpr auto count() const noexcept
        {
                return this->do_count();
        }

private:
        constexpr decltype(auto) block_ref(key_type n)
        {
                return *(this->block_ptr(n));
        }

        constexpr decltype(auto) block_ref(key_type n) const
        {
                return *(this->block_ptr(n));
        }

        static constexpr auto mask(key_type n)
        {
                return Block{1} << detail::storage<Block>::shift_idx(n);
        }

        constexpr auto is_mask(key_type n) const
        {
                return (block_ref(n) & mask(n)) != Block{0};
        }
};

template<class T, class Block, int Nb>
constexpr auto swap(bit_set<T, Block, Nb>& lhs, bit_set<T, Block, Nb>& rhs) noexcept
{
        lhs.swap(rhs);
        return;
}

template<class T, class Block, int Nb>
constexpr decltype(auto) begin(bit_set<T, Block, Nb> const& s) noexcept
{
        return s.begin();
}

template<class T, class Block, int Nb>
constexpr decltype(auto) end(bit_set<T, Block, Nb> const& s) noexcept
{
        return s.end();
}

template<class T, class Block, int Nb>
constexpr decltype(auto) cbegin(bit_set<T, Block, Nb> const& s) noexcept
{
        return s.cbegin();
}

template<class T, class Block, int Nb>
constexpr decltype(auto) cend(bit_set<T, Block, Nb> const& s) noexcept
{
        return s.cend();
}

}       // namespace bit

using BitSet = bit::bit_set<int, uint64_t, 2>;

}       // namespace dctl
