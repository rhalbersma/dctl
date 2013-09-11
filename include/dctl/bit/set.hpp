#pragma once
#include <climits>                      // CHAR_BIT
#include <cstddef>                      // ptrdiff_t, size_t
#include <cstdint>                      // uint64_t
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

template<int, class = void> class set;

template<class _>
class set<1, _>
{
public:
        using storage_type = uint64_t;
        using key_type = int;
        using value_type = int;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<1, _>;
        using const_reference = reference;
        using iterator = bit_iterator<1, _>;
        using const_iterator = iterator;

        set() = default;

        explicit set(storage_type const& data)
        :
                data_{data}
        {
                BOOST_ASSERT(invariant());
        }

        template<class InputIt>
        set(InputIt first, InputIt last)
        {
                BOOST_ASSERT(empty());
                insert(first, last);
        }

        set(std::initializer_list<value_type> ilist)
        {
                BOOST_ASSERT(empty());
                insert(ilist);
        }

        set& operator=(std::initializer_list<value_type> ilist)
        {
                clear();
                insert(ilist);
                return *this;
        }

        // iterators

        iterator begin() noexcept
        {
                return iterator{&data_};
        }

        const_iterator begin() const noexcept
        {
                return const_iterator{&data_};
        }

        const_iterator cbegin() const noexcept
        {
                return begin();
        }

        iterator end() noexcept
        {
                return iterator{&data_, 64};
        }

        const_iterator end() const noexcept
        {
                return const_iterator{&data_, 64};
        }

        const_iterator cend() const noexcept
        {
                return end();
        }

        // capacity

        bool empty() const noexcept
        {
                return data_ == 0;
        }

        size_type size() const noexcept
        {
                return static_cast<size_type>(bit::size(data_));
        }

        size_type max_size() const noexcept
        {
                return CHAR_BIT * sizeof(storage_type);
        }

        // modifiers

        template<class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template <class... Args>
        iterator emplace_hint(const_iterator /* position */, Args&&... args);

        std::pair<iterator, bool> insert(value_type const& value)
        {
                data_ |= element(value);
                BOOST_ASSERT(invariant());
                return std::make_pair(iterator{&data_, value}, true);
        }

        std::pair<iterator, bool> insert(value_type&& value)
        {
                data_ |= element(std::move(value));
                BOOST_ASSERT(invariant());
                return std::make_pair(iterator{&data_, value}, true);
        }

        iterator insert(const_iterator /*hint*/, value_type value)
        {
                insert(value);
                return iterator{&data_, value};
        }

        template<class InputIt>
        void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        insert(*it);
        }

        void insert(std::initializer_list<value_type> ilist)
        {
                insert(ilist.begin(), ilist.end());
        }

        void erase(iterator pos)
        {
                erase(*pos);
        }

        void erase(iterator first, iterator last)
        {
                for (auto it = first; it != last; ++it)
                        erase(*it);
        }

        void erase(key_type const& key)
        {
                data_ &= ~element(key);
                BOOST_ASSERT(invariant());
        }

        void swap(set& other)
        {
                using std::swap;
                swap(data_, other.data_);
                BOOST_ASSERT(invariant());
        }

        void clear() noexcept
        {
                data_ = 0;
                BOOST_ASSERT(empty());
                BOOST_ASSERT(invariant());
        }

        bool count(key_type const& key) const
        {
                return (data_ & element(key)) != 0;
        }

        iterator find(key_type const& key)
        {
                auto result = data_ & element(key);
                BOOST_ASSERT(invariant());
                return result? iterator{&data_, key} : end();
        }

        const_iterator find(key_type const& key) const
        {
                auto result = data_ & element(key);
                return result? const_iterator{&data_, key} : end();
        }

        friend bool operator==(set const& lhs, set const& rhs)
        {
                return lhs.data_ == rhs.data_;
        }

        friend bool operator!=(set const& lhs, set const& rhs)
        {
                return !(lhs == rhs);
        }

        friend bool operator<(set const& lhs, set const& rhs)
        {
                return lhs.data_ < rhs.data_;
        }

        friend bool operator>=(set const& lhs, set const& rhs)
        {
                return !(lhs < rhs);
        }

        friend bool operator>(set const& lhs, set const& rhs)
        {
                return rhs < lhs;
        }

        friend bool operator<=(set const& lhs, set const& rhs)
        {
                return !(rhs < lhs);
        }

        set& operator&=(set const& other)
        {
                data_ &= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        friend set operator&(set const& lhs, set const& rhs)
        {
                set nrv{lhs};
                nrv &= rhs;
                return nrv;
        }

        set& operator|=(set const& other)
        {
                data_ |= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        friend set operator|(set const& lhs, set const& rhs)
        {
                set nrv{lhs};
                nrv |= rhs;
                return nrv;
        }

        set& operator^=(set const& other)
        {
                data_ ^= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        friend set operator^(set const& lhs, set const& rhs)
        {
                set nrv{lhs};
                nrv ^= rhs;
                return nrv;
        }

        set& operator<<=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ <<= n;
                BOOST_ASSERT(invariant());
                return *this;
        }

        friend set operator<<(set const& lhs, std::size_t n)
        {
                set nrv{lhs};
                nrv <<= n;
                return nrv;
        }

        set& operator>>=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ >>= n;
                BOOST_ASSERT(invariant());
                return *this;
        }

        friend set operator>>(set const& lhs, std::size_t n)
        {
                set nrv{lhs};
                nrv >>= n;
                return nrv;
        }

        set& flip()
        {
                data_ = ~data_;
                return *this;
        }

        friend set operator~(set const& lhs)
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
void swap(set<N>& lhs, set<N>& rhs)
{
        lhs.swap(rhs);
}

template<int N>
auto begin(set<N> const& s) -> decltype(s.begin())
{
        return s.begin();
}

template<int N>
auto end(set<N> const& s) -> decltype(s.end())
{
        return s.end();
}

template<int N>
auto empty(set<N> const& s) -> decltype(s.empty())
{
        return s.empty();
}

}       // namespace bit
}       // namespace dctl
