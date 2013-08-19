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
#include <boost/operators.hpp>          // totally_ordered, bitwise, shiftable
#include <dctl/bit/iterator.hpp>        // bit_iterator
#include <dctl/bit/reference.hpp>       // bit_reference
#include <dctl/bit/raw.hpp>             // size

namespace dctl {
namespace bit {

template<class Key, class Compare = std::less<Key>, class Storage = uint64_t>
class set;

template<class Key, class Compare>
class set<Key, Compare, uint64_t>
:       boost::totally_ordered< set<Key, Compare, uint64_t>
,       boost::bitwise< set<Key, Compare, uint64_t>
,       boost::shiftable< set<Key, Compare, uint64_t>, std::size_t
> > >
{
public:
        using storage_type = uint64_t;
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<key_type, storage_type>;
        using const_reference = reference;
        using iterator = bit_iterator<key_type, storage_type>;
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
                return iterator{data_};
        }

        const_iterator begin() const noexcept
        {
                return const_iterator{data_};
        }

        const_iterator cbegin() const noexcept
        {
                return begin();
        }

        iterator end() noexcept
        {
                return iterator{};
        }

        const_iterator end() const noexcept
        {
                return const_iterator{};
        }

        const_iterator cend() const noexcept
        {
                return end();
        }

        // capacity

        bool empty() const noexcept
        {
                return begin() == end();
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
                return std::make_pair(iterator{data_}, true);
        }

        std::pair<iterator, bool> insert(value_type&& value)
        {
                data_ |= element(std::move(value));
                BOOST_ASSERT(invariant());
                return std::make_pair(iterator{data_}, true);
        }

        iterator insert(const_iterator /*hint*/, value_type value)
        {
                insert(value);
                return iterator{data_};
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
                return result? iterator{result} : end();
        }

        const_iterator find(key_type const& key) const
        {
                auto result = data_ & element(key);
                return result? const_iterator{result} : end();
        }

        // operator!= provided by boost::totally_ordered
        friend bool operator==(set const& lhs, set const& rhs)
        {
                return lhs.data_ == rhs.data_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(set const& lhs, set const& rhs)
        {
                return lhs.data_ < rhs.data_;
        }

        // operator& provided by boost::bitwise
        set& operator&=(set const& other)
        {
                data_ &= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        // operator| provided by boost::bitwise
        set& operator|=(set const& other)
        {
                data_ |= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        // operator^ provided by boost::bitwise
        set& operator^=(set const& other)
        {
                data_ ^= other.data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

        // operator<< provided by boost::shiftable
        set& operator<<=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ <<= n;
                BOOST_ASSERT(invariant());
                return *this;
        }

        // operator>> provided by boost::shiftable
        set& operator>>=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ >>= n;
                BOOST_ASSERT(invariant());
                return *this;
        }

        set& flip()
        {
                data_ = ~data_;
                BOOST_ASSERT(invariant());
                return *this;
        }

private:
        bool invariant() const
        {
                return (
                        std::distance(begin(), end()) == static_cast<std::ptrdiff_t>(size()) &&
                        std::is_sorted(begin(), end(), key_compare()) &&
                        is_unique()
                );
        }

        bool is_unique() const
        {
                return std::adjacent_find(begin(), end(), [](key_type const& lhs, key_type const& rhs){
                        return !key_compare()(lhs, rhs) && !key_compare()(rhs, lhs);
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

template<class Key, class Compare, class Storage>
set<Key, Compare, Storage> operator~(set<Key, Compare, Storage> const& lhs)
{
        return set<Key, Compare, Storage>{lhs}.flip();
}

template<class Key, class Compare, class Storage>
void swap(set<Key, Compare, Storage>& lhs, set<Key, Compare, Storage>& rhs)
{
        lhs.swap(rhs);
}

template<class Key, class Compare, class Storage>
auto begin(set<Key, Compare, Storage> const& s) -> decltype(s.begin())
{
        return s.begin();
}

template<class Key, class Compare, class Storage>
auto end(set<Key, Compare, Storage> const& s) -> decltype(s.end())
{
        return s.end();
}

template<class Key, class Compare, class Storage>
auto empty(set<Key, Compare, Storage> const& s) -> decltype(s.empty())
{
        return s.empty();
}

}       // namespace bit
}       // namespace dctl
