#pragma once
#include <cstddef>                      // ptrdiff_t, size_t
#include <cstdint>                      // uint64_t, CHAR_BIT
#include <algorithm>                    // is_sorted
#include <functional>                   // less
#include <initializer_list>             // initializer_list
#include <iterator>                     // distance
#include <utility>                      // swap
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/bit/iterator.hpp>        // bit_iterator
#include <dctl/bit/reference.hpp>       // bit_reference
#include <dctl/bit/raw.hpp>             // size

namespace dctl {
namespace bit {

template<int> class set;

template<>
class set<1>
{
public:
        using storage_type = uint64_t;
        static constexpr auto N = static_cast<int>(CHAR_BIT * sizeof(storage_type));

        using key_type = int;
        using value_type = int;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using reference = bit_reference<1>;
        using const_reference = reference;
        using iterator = bit_iterator<1>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr set() = default;

        constexpr explicit set(storage_type const& data) noexcept
        :
                data_{data}
        {}

        template<class InputIt>
        constexpr set(InputIt first, InputIt last)
        {
                insert(first, last);
        }

        constexpr set(std::initializer_list<value_type> ilist)
        {
                insert(ilist);
        }

        constexpr auto& operator=(std::initializer_list<value_type> ilist)
        {
                clear();
                insert(ilist);
                return *this;
        }

        // iterators

        constexpr auto begin() noexcept
        {
                return iterator{&data_};
        }

        constexpr auto begin() const noexcept
        {
                return const_iterator{&data_};
        }

        constexpr auto end() noexcept
        {
                return iterator{&data_, N};
        }

        constexpr auto end() const noexcept
        {
                return const_iterator{&data_, N};
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

        constexpr size_type size() const noexcept
        {
                return static_cast<size_type>(bit::intrinsic::size(data_));
        }

        constexpr auto max_size() const noexcept
        {
                return CHAR_BIT * sizeof(storage_type);
        }

        // modifiers

        template<class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template <class... Args>
        iterator emplace_hint(const_iterator /* position */, Args&&... args);

        constexpr auto insert(value_type const& value)
        {
                data_ |= element(value);
                return std::make_pair(iterator{&data_, value}, true);
        }

        constexpr auto insert(value_type&& value)
        {
                data_ |= element(std::move(value));
                return std::make_pair(iterator{&data_, value}, true);
        }

        constexpr iterator insert(const_iterator /*hint*/, value_type value)
        {
                insert(value);
                return iterator{&data_, value};
        }

        template<class InputIt>
        constexpr void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        insert(*it);
        }

        constexpr void insert(std::initializer_list<value_type> ilist)
        {
                insert(ilist.begin(), ilist.end());
        }

        constexpr void erase(iterator pos)
        {
                erase(*pos);
        }

        constexpr void erase(iterator first, iterator last)
        {
                for (auto it = first; it != last; ++it)
                        erase(*it);
        }

        constexpr void erase(key_type const& key)
        {
                data_ &= ~element(key);
        }

        constexpr void swap(set& other) noexcept
        {
                using std::swap;
                swap(data_, other.data_);
        }

        constexpr void clear() noexcept
        {
                data_ = 0;
        }

        constexpr auto count(key_type const& key) const
        {
                return (data_ & element(key)) != 0;
        }

        constexpr auto find(key_type const& key)
        {
                auto result = data_ & element(key);
                return result? iterator{&data_, key} : end();
        }

        constexpr auto find(key_type const& key) const
        {
                auto result = data_ & element(key);
                return result? const_iterator{&data_, key} : end();
        }

        friend constexpr bool operator==(set const& lhs, set const& rhs) noexcept
        {
                return lhs.data_ == rhs.data_;
        }

        friend constexpr bool operator!=(set const& lhs, set const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        friend constexpr bool operator<(set const& lhs, set const& rhs) noexcept
        {
                return lhs.data_ < rhs.data_;
        }

        friend constexpr bool operator>=(set const& lhs, set const& rhs) noexcept
        {
                return !(lhs < rhs);
        }

        friend constexpr bool operator>(set const& lhs, set const& rhs) noexcept
        {
                return rhs < lhs;
        }

        friend bool operator<=(set const& lhs, set const& rhs) noexcept
        {
                return !(rhs < lhs);
        }

        constexpr auto& operator&=(set const& other) noexcept
        {
                data_ &= other.data_;
                return *this;
        }

        friend constexpr set operator&(set const& lhs, set const& rhs) noexcept
        {
                set nrv{lhs};
                nrv &= rhs;
                return nrv;
        }

        constexpr auto& operator|=(set const& other) noexcept
        {
                data_ |= other.data_;
                return *this;
        }

        friend constexpr set operator|(set const& lhs, set const& rhs) noexcept
        {
                set nrv{lhs};
                nrv |= rhs;
                return nrv;
        }

        constexpr auto& operator^=(set const& other) noexcept
        {
                data_ ^= other.data_;
                return *this;
        }

        friend constexpr set operator^(set const& lhs, set const& rhs) noexcept
        {
                set nrv{lhs};
                nrv ^= rhs;
                return nrv;
        }

        constexpr auto& operator<<=(std::size_t n)
        {
                data_ <<= n;
                return *this;
        }

        friend constexpr set operator<<(set const& lhs, std::size_t n)
        {
                set nrv{lhs};
                nrv <<= n;
                return nrv;
        }

        constexpr auto& operator>>=(std::size_t n)
        {
                data_ >>= n;
                return *this;
        }

        friend constexpr set operator>>(set const& lhs, std::size_t n)
        {
                set nrv{lhs};
                nrv >>= n;
                return nrv;
        }

        constexpr auto& flip() noexcept
        {
                data_ = ~data_;
                return *this;
        }

        friend constexpr set operator~(set const& lhs) noexcept
        {
                set nrv{lhs};
                nrv.flip();
                return nrv;
        }

private:
        bool invariant() const
        {
                return (
                        std::distance(begin(), end()) == static_cast<std::ptrdiff_t>(size()) &&
                        std::is_sorted(begin(), end()) &&
                        is_unique()
                );
        }

        bool is_unique() const
        {
                return std::adjacent_find(begin(), end(), [](key_type const& lhs, key_type const& rhs){
                        return !(lhs < rhs) && !(rhs < lhs);
                }) == end();
        }

        storage_type element(key_type const& key) const
        {
                BOOST_ASSERT(key < static_cast<key_type>(max_size()));
                return storage_type{1} << key;
        }

        // representation

        storage_type data_ = 0;
};

template<int N>
constexpr auto swap(set<N>& lhs, set<N>& rhs) noexcept
{
        lhs.swap(rhs);
        return;
}

template<int N>
constexpr auto begin(set<N> const& s) noexcept -> decltype(s.begin())
{
        return s.begin();
}

template<int N>
constexpr auto end(set<N> const& s) noexcept -> decltype(s.end())
{
        return s.end();
}

template<int N>
constexpr auto cbegin(set<N> const& s) noexcept -> decltype(s.cbegin())
{
        return s.cbegin();
}

template<int N>
constexpr auto cend(set<N> const& s) noexcept -> decltype(s.cend())
{
        return s.cend();
}

}       // namespace bit
}       // namespace dctl
