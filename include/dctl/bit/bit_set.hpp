#pragma once
#include <cassert>                              // assert
#include <cstddef>                              // ptrdiff_t, size_t
#include <cstdint>                              // uint64_t, CHAR_BIT
#include <initializer_list>                     // initializer_list
#include <iterator>                             // distance
#include <stdexcept>                            // out_of_range
#include <utility>                              // swap
#include <dctl/bit/iterator.hpp>                // bit_iterator
#include <dctl/bit/reference.hpp>               // bit_reference
#include <dctl/bit/detail/base_set.hpp>

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class bit_set
:
        private detail::base_set<T, Block, Nb>
{
public:
        using Base = detail::base_set<T, Block, Nb>;
        static constexpr auto N = Nb * detail::storage<Block>::size;

        using key_type = T;
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<T, Block, Nb>;
        using const_reference = reference;
        using iterator = bit_iterator<T, Block, Nb>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // structors

        constexpr bit_set() noexcept = default;

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

        constexpr auto end() noexcept
        {
                return iterator{this->block_ptr(N), N};
        }

        constexpr auto end() const noexcept
        {
                return const_iterator{this->block_ptr(N), N};
        }

        auto rbegin() noexcept
        {
                return reverse_iterator(end());
        }

        auto rbegin() const noexcept
        {
                return const_reverse_iterator(end());
        }

        auto rend() noexcept
        {
                return reverse_iterator(begin());
        }

        auto rend() const noexcept
        {
                return const_reverse_iterator(begin());
        }

        constexpr auto cbegin() const noexcept
        {
                return begin();
        }

        constexpr auto cend() const noexcept
        {
                return end();
        }

        auto crbegin() const noexcept
        {
                return rbegin();
        }

        auto crend() const noexcept
        {
                return rend();
        }

        // raw data

        constexpr auto const& data() const noexcept
        {
                return this->data_;
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

        constexpr auto insert(value_type const& value)
        {
                auto const not_set = !is_mask(value);
                if (not_set)
                        set(value);
                return std::make_pair(iterator{this->block_ptr(value), value}, not_set);
        }

        constexpr auto insert(const_iterator /*hint*/, value_type const& value)
        {
                set(value);
                return iterator{this->block_ptr(value), value};
        }

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        set(*it);
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
                return found? iterator{this->block(key), key} : end();
        }

        constexpr auto find(key_type const& key) const
        {
                auto const found = is_mask(key);
                return found? const_iterator{this->block(key), key} : cend();
        }

        // relational operators

        friend constexpr bool operator==(bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return Base::do_equal(lhs, rhs);
        }

        friend constexpr auto operator!=(bit_set const& lhs, bit_set const& rhs) noexcept -> bool
        {
                return !(lhs == rhs);
        }

        friend constexpr bool operator<(bit_set const& lhs, bit_set const& rhs) noexcept
        {
                return Base::do_lexicograhical_compare(lhs, rhs);
        }

        friend constexpr auto operator>=(bit_set const& lhs, bit_set const& rhs) noexcept -> bool
        {
                return !(lhs < rhs);
        }

        friend constexpr auto operator>(bit_set const& lhs, bit_set const& rhs) noexcept -> bool
        {
                return rhs < lhs;
        }

        friend constexpr auto operator<=(bit_set const& lhs, bit_set const& rhs) noexcept -> bool
        {
                return !(rhs < lhs);
        }

        // bit access

        constexpr auto operator[](key_type n)
        {
                return reference{this->block(n), Base::index(n)};
        }

        constexpr auto operator[](key_type n) const
        {
                return is_mask(n);
        }

        constexpr auto test(key_type n) const
        {
                if (!(0 <= n && n < detail::storage<Block>::size))
                        throw std::out_of_range("");
                return operator[](n);
        }

        constexpr auto& reset(key_type n)
        {
                this->block(n) &= ~mask(n);
                return *this;
        }

        constexpr auto& set(key_type n, bool value = true)
        {
                if (value)
                        this->block(n) |= mask(n);
                else
                        this->block(n) &= ~mask(n);
                return *this;
        }

        constexpr auto& flip(key_type n)
        {
                this->block(n) ^= mask(n);
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

        constexpr auto& operator<<=(std::size_t n)
        {
                this->do_left_shift(n);
                return *this;
        }

        constexpr auto& operator>>=(std::size_t n)
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

        friend constexpr auto operator<<(bit_set const& lhs, std::size_t n) -> bit_set
        {
                bit_set nrv{lhs};
                nrv <<= n;
                return nrv;
        }

        friend constexpr auto operator>>(bit_set const& lhs, std::size_t n) -> bit_set
        {
                bit_set nrv{lhs};
                nrv >>= n;
                return nrv;
        }

        // bitwise algorithms

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

        constexpr auto count() const noexcept
        {
                return this->do_count();
        }

private:
        constexpr auto& block(key_type n) noexcept
        {
                return *(this->block_ptr(n));
        }

        constexpr auto const& block(key_type n) const noexcept
        {
                return *(this->block_ptr(n));
        }

        static constexpr auto mask(key_type n)
        {
                return Block{1} << Base::index(n);
        }

        static constexpr auto is_mask(key_type n)
        {
                return mask(n) == 0;
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
}       // namespace dctl
