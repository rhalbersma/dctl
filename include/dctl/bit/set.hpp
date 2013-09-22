#pragma once
#include <cassert>                      // assert
#include <cstddef>                      // ptrdiff_t, size_t
#include <cstdint>                      // uint64_t, CHAR_BIT
#include <algorithm>                    // is_sorted
#include <functional>                   // less
#include <initializer_list>             // initializer_list
#include <iterator>                     // distance
#include <utility>                      // swap
#include <dctl/bit/iterator.hpp>        // bit_iterator
#include <dctl/bit/reference.hpp>       // bit_reference
#include <dctl/bit/detail/base_set_1.hpp>

namespace dctl {
namespace bit {

template<int N>
class set
:
        private detail::base_set<uint64_t, 1>
{
public:
        using WordT = uint64_t;
        static constexpr auto Nw = 1;//(N - 1) / (CHAR_BIT * sizeof(WordT)) + 1;

        using key_type = int;
        using value_type = int;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<WordT, Nw>;
        using const_reference = reference;
        using iterator = bit_iterator<WordT, Nw>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // structors

        constexpr set() noexcept = default;

        template<class InputIt>
        constexpr set(InputIt first, InputIt last)
        {
                insert(first, last);
        }

        constexpr set(std::initializer_list<value_type> ilist)
        :
                set{ilist.begin(), ilist.end()}
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
                return iterator{this->begin_ptr()};
        }

        constexpr auto begin() const noexcept
        {
                return const_iterator{this->begin_ptr()};
        }

        constexpr auto end() noexcept
        {
                return iterator{this->end_ptr(), N};
        }

        constexpr auto end() const noexcept
        {
                return const_iterator{this->end_ptr(), N};
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

        // capacity

        constexpr auto empty() const noexcept
        {
                return data_ == 0;
        }

        constexpr auto size() const noexcept
        {
                return static_cast<size_type>(bit::intrinsic::size(data_));
        }

        constexpr auto max_size() const noexcept
        {
                return N;
        }

        // modifiers

        constexpr auto insert(value_type const& value)
        {
                auto const present = is_mask(value);
                if (!present)
                        set_mask(value);
                return std::make_pair(iterator{this->word_ptr(value), this->word_offset(value)}, !present);
        }

        constexpr auto insert(const_iterator /*hint*/, value_type const& value)
        {
                set_mask(value);
                return iterator{this->word_ptr(value), this->word_offset(value)};
        }

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        set_mask(*it);
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
                auto const n = static_cast<size_type>(is_mask(key));
                clear_mask(key);
                return n;
        }

        constexpr const_iterator erase(const_iterator pos)
        {
                clear_mask(*pos++);
                return pos;
        }

        constexpr auto erase(const_iterator first, const_iterator last)
        {
                auto it = first;
                while (it != last)
                        clear_mask(*it++);
                return it;
        }

        constexpr void swap(set& other) noexcept
        {
                using std::swap;
                swap(this->data_, other.data_);
        }

        constexpr void clear() noexcept
        {
                this->do_clear();
        }

        constexpr auto count(key_type const& key) const
        {
                return static_cast<size_type>(is_mask(key));
        }

        constexpr auto find(key_type const& key)
        {
                auto const result = is_mask(key);
                return result? iterator{this->word_ptr(key), this->word_offset(key)} : end();
        }

        constexpr auto find(key_type const& key) const
        {
                auto const result = is_mask(key);
                return result? const_iterator{this->word_ptr(key), this->word_offset(key)} : cend();
        }

        friend constexpr auto operator==(set const& lhs, set const& rhs) noexcept -> bool
        {
                return lhs.data_ == rhs.data_;
        }

        friend constexpr auto operator!=(set const& lhs, set const& rhs) noexcept -> bool
        {
                return !(lhs == rhs);
        }

        friend constexpr auto operator<(set const& lhs, set const& rhs) noexcept -> bool
        {
                return lhs.data_ < rhs.data_;
        }

        friend constexpr auto operator>=(set const& lhs, set const& rhs) noexcept -> bool
        {
                return !(lhs < rhs);
        }

        friend constexpr auto operator>(set const& lhs, set const& rhs) noexcept -> bool
        {
                return rhs < lhs;
        }

        friend constexpr auto operator<=(set const& lhs, set const& rhs) noexcept -> bool
        {
                return !(rhs < lhs);
        }

        constexpr auto& flip() noexcept
        {
                this->do_flip();
                return *this;
        }

        constexpr auto& operator&=(set const& other) noexcept
        {
                this->do_and(other);
                return *this;
        }

        constexpr auto& operator|=(set const& other) noexcept
        {
                this->do_or(other);
                return *this;
        }

        constexpr auto& operator^=(set const& other) noexcept
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

        friend constexpr auto operator~(set const& lhs) noexcept -> set
        {
                set nrv{lhs};
                nrv.flip();
                return nrv;
        }

        friend constexpr auto operator&(set const& lhs, set const& rhs) noexcept -> set
        {
                set nrv{lhs};
                nrv &= rhs;
                return nrv;
        }

        friend constexpr auto operator|(set const& lhs, set const& rhs) noexcept -> set
        {
                set nrv{lhs};
                nrv |= rhs;
                return nrv;
        }

        friend constexpr auto operator^(set const& lhs, set const& rhs) noexcept -> set
        {
                set nrv{lhs};
                nrv ^= rhs;
                return nrv;
        }

        friend constexpr auto operator<<(set const& lhs, std::size_t n) -> set
        {
                set nrv{lhs};
                nrv <<= n;
                return nrv;
        }

        friend constexpr auto operator>>(set const& lhs, std::size_t n) -> set
        {
                set nrv{lhs};
                nrv >>= n;
                return nrv;
        }

private:
        constexpr auto mask(key_type n) const
        {
                return WordT{1} << n;
        }

        constexpr void set_mask(key_type n)
        {
                this->data_ |= mask(n);
        }

        constexpr void clear_mask(key_type n)
        {
                this->data_ &= ~mask(n);
        }

        constexpr auto is_mask(key_type n) const -> bool
        {
                return (this->data_ & mask(n)) != 0;
        }
};

template<int N>
constexpr auto swap(set<N>& lhs, set<N>& rhs) noexcept
{
        lhs.swap(rhs);
        return;
}

template<int N>
constexpr decltype(auto) begin(set<N> const& s) noexcept
{
        return s.begin();
}

template<int N>
constexpr decltype(auto) end(set<N> const& s) noexcept
{
        return s.end();
}

template<int N>
constexpr decltype(auto) cbegin(set<N> const& s) noexcept
{
        return s.cbegin();
}

template<int N>
constexpr decltype(auto) cend(set<N> const& s) noexcept
{
        return s.cend();
}

}       // namespace bit
}       // namespace dctl
