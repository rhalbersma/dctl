#pragma once
#include <cstddef>                      // ptrdiff_t, size_t
#include <cstdint>                      // uint64_t
#include <algorithm>                    // is_sorted
#include <initializer_list>             // initializer_list
#include <iterator>                     // distance
#include <utility>                      // swap
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered, bitwise, shiftable
#include <dctl/bit/iterator.hpp>        // bit_iterator, bit_reference
#include <dctl/bit/raw.hpp>             // size

namespace dctl {
namespace bit {

template<class T, class U = uint64_t, std::size_t N = 1>
class set;

template<class T, class U>
class set<T, U, 1>
:       boost::totally_ordered< set<T, U, 1>
,       boost::bitwise< set<T, U, 1>
,       boost::shiftable< set<T, U, 1>, std::size_t
> > >
{
public:
        using block_type = U;
        using key_type = T;
        typedef T value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef bit_reference<T, U> reference;
        typedef bit_reference<T, U> const_reference;
        typedef bit_iterator<T, U> iterator;
        typedef bit_iterator<T, U> const_iterator;

        set()
        :
                data_()
        {
                BOOST_ASSERT(invariant());
        }

        explicit set(block_type const& b)
        :
                data_(b)
        {
                BOOST_ASSERT(invariant());
        }

        template<class InputIt>
        set(InputIt first, InputIt last)
        :
                data_(0)
        {
                BOOST_ASSERT(empty());
                for (auto it = first; it != last; ++it)
                         insert(*it);
                BOOST_ASSERT(invariant());
        }

        set(std::initializer_list<value_type> init)
        :
                data_(0)
        {
                BOOST_ASSERT(empty());
                for (auto i: init)
                        insert(i);
                BOOST_ASSERT(invariant());
        }

        iterator begin()
        {
                return iterator(data_);
        }

        const_iterator begin() const
        {
                return const_iterator(data_);
        }

        const_iterator cbegin() const
        {
                return const_iterator(data_);
        }

        iterator end()
        {
                return iterator();
        }

        const_iterator end() const
        {
                return const_iterator();
        }

        const_iterator cend() const
        {
                return const_iterator();
        }

        bool empty() const
        {
                return begin() == end();
        }

        size_type size() const
        {
                return static_cast<size_type>(bit::size(data_));
        }

        size_type max_size() const
        {
                return 8 * sizeof(block_type);
        }

        void clear()
        {
                data_ = 0;
                BOOST_ASSERT(empty());
                BOOST_ASSERT(invariant());
        }

        std::pair<iterator, bool> insert(value_type const& value)
        {
                data_ |= element(value);
                BOOST_ASSERT(invariant());
                return std::make_pair(iterator(data_), true);
        }

        std::pair<iterator, bool> insert(value_type&& value)
        {
                data_ |= element(std::move(value));
                BOOST_ASSERT(invariant());
                return std::make_pair(iterator(data_), true);
        }

        iterator insert(const_iterator /*hint*/, value_type value)
        {
                insert(value);
                BOOST_ASSERT(invariant());
                return iterator(data_);
        }

        template<class InputIt>
        void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        insert(*it);
                BOOST_ASSERT(invariant());
        }

        void insert(std::initializer_list<value_type> const& ilist)
        {
                for (auto const& i: ilist)
                        insert(i);
                BOOST_ASSERT(invariant());
        }

        void erase(iterator pos)
        {
                erase(*pos);
        }

        void erase(iterator first, iterator last)
        {
                for (auto it = first; it != last; ++it)
                        erase(*it);
                BOOST_ASSERT(invariant());
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

        bool count(key_type const& key) const
        {
                return (data_ & element(key)) != 0;
        }

        iterator find(key_type const& key)
        {
                auto result = data_ & element(key);
                BOOST_ASSERT(invariant());
                return result? iterator(result) : end();
        }

        const_iterator find(key_type const& key) const
        {
                auto result = data_ & element(key);
                return result? const_iterator(&data_, result) : end();
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
                        std::is_sorted(begin(), end()) &&
                        std::distance(begin(), end()) == static_cast<std::ptrdiff_t>(size())
                );
        }

        block_type element(key_type const& key) const
        {
                BOOST_ASSERT(key < static_cast<key_type>(max_size()));
                return block_type(1) << key;
        }

        block_type data_;
};

template<class T, class U, std::size_t N>
set<T, U, N> operator~(set<T, U, N> const& lhs)
{
        return set<T, U, N>(lhs).flip();
}

template<class T, class U, std::size_t N>
void swap(set<T, U, N>& lhs, set<T, U, N>& rhs)
{
        lhs.swap(rhs);
}

template<class T, class U, std::size_t N>
auto begin(set<T, U, N> const& s) -> decltype(s.begin())
{
        return s.begin();
}

template<class T, class U, std::size_t N>
auto end(set<T, U, N> const& s) -> decltype(s.end())
{
        return s.end();
}

template<class T, class U, std::size_t N>
auto empty(set<T, U, N> const& s) -> decltype(s.empty())
{
        return s.empty();
}

template<class T, class U, std::size_t N>
auto size(set<T, U, N> const& s) -> decltype(s.size())
{
        return s.size();
}

}       // namespace bit
}       // namespace dctl
