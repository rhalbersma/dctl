#pragma once
#include <cstddef>                      // size_t
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <utility>
#include <boost/operators.hpp>          // totally_ordered

namespace dctl {

template<typename> class bit_const_reference;
template<typename> class bit_const_iterator;
template<typename> class bit_reference;
template<typename> class bit_iterator;

template<typename T>
std::size_t safe_ctzll(T mask)
{
     return mask? __builtin_ctzll(mask) : (8 * sizeof(T));
}

template<typename T>
std::size_t safe_clzll(T mask)
{
     return mask? __builtin_clzll(mask) : (8 * sizeof(T));
}

template<typename T>
class bit_const_reference
{
public:
        bit_const_reference(T const& p, T m)
        :
                pseg_(p),
                mask_(m)
        {}

        // reference cannot be rebound
        bit_const_reference& operator=(bit_const_reference const&) = delete;

        operator std::size_t() const
        {
                return safe_ctzll(mask_);
        }

        bit_const_iterator<T> operator&() const
        {
                return bit_const_iterator<T>(&pseg_, mask_);
        }

private:
        T const& pseg_;
        T mask_;
};

template<typename Block>
class bit_const_iterator
:
        public std::iterator
        <
                std::bidirectional_iterator_tag,
                std::size_t,
                std::ptrdiff_t,
                bit_const_iterator<Block>,
                bit_const_reference<Block>
        >
{
public:
        bit_const_iterator(Block const* p, Block m)
        :
                pseg_(p),
                mask_(m)
        {}

        bit_const_reference<Block> operator*() const
        {
                return bit_const_reference<Block>(*pseg_, mask_);
        }

        bit_const_iterator& operator++()
        {
                auto ctz = safe_ctzll(mask_);
                mask_ ^= Block(1) << ctz;
                return *this;
        }

        bit_const_iterator operator++(int)
        {
                auto old = *this;
                ++*this;
                return old;
        }

        bit_const_iterator& operator--()
        {
                auto clz = safe_clzll(mask_);
                mask_ ^= Block(1) << clz;
                return *this;
        }

        bit_const_iterator operator--(int)
        {
                auto old = *this;
                --*this;
                return old;
        }

        friend bool operator==(bit_const_iterator const& lhs, bit_const_iterator const& rhs)
        {
                return lhs.pseg_ == rhs.pseg_ && lhs.mask_ == rhs.mask_;
        }

        friend bool operator!=(bit_const_iterator const& lhs, bit_const_iterator const& rhs)
        {
                return !(lhs == rhs);
        }

private:
        Block const* pseg_;
        Block mask_;
};

template<typename T>
class bit_reference
{
public:
        bit_reference(T const& p, T m)
        :
                pseg_(p),
                mask_(m)
        {}

        // reference cannot be rebound
        bit_reference& operator=(bit_reference const&) = delete;

        operator std::size_t() const
        {
                // guard against invalidation
                return safe_ctzll(pseg_ & mask_);
        }

        bit_iterator<T> operator&() const
        {
                return bit_iterator<T>(&pseg_, mask_);
        }

private:
        T const& pseg_;
        T mask_;
};

template<typename T>
class bit_iterator
:
        public std::iterator
        <
                std::bidirectional_iterator_tag,
                std::size_t,
                std::ptrdiff_t,
                bit_iterator<T>,
                bit_reference<T>
        >
{
public:
        bit_iterator(T const* p, T m)
        :
                pseg_(p),
                mask_(m)
        {}

        operator bit_const_iterator<T>() const
        {
               return bit_const_iterator<T>(pseg_, mask_);
        }

        bit_reference<T> operator*() const
        {
                return bit_reference<T>(*pseg_, mask_);
        }

        bit_iterator& operator++()
        {
                auto ctz = safe_ctzll(mask_);
                mask_ ^= T(1) << ctz;
                return *this;
        }

        bit_iterator operator++(int)
        {
                auto old = *this;
                ++*this;
                return old;
        }

        bit_iterator& operator--()
        {
                auto clz = safe_clzll(mask_);
                mask_ ^= T(1) << clz;
                return *this;
        }

        bit_iterator operator--(int)
        {
                auto old = *this;
                --*this;
                return old;
        }

        friend bool operator==(bit_iterator const& lhs, bit_iterator const& rhs)
        {
                return lhs.pseg_ == rhs.pseg_ && lhs.mask_ == rhs.mask_;
        }

        friend bool operator!=(bit_iterator const& lhs, bit_iterator const& rhs)
        {
                return !(lhs == rhs);
        }

private:
        T const* pseg_;
        T mask_;
};

template<typename Block = uint64_t, std::size_t N = 1>
struct bit_set;

template<typename Block>
struct bit_set<Block, 1>
:       boost::totally_ordered< bit_set<Block, 1>
,       boost::bitwise< bit_set<Block, 1>
,       boost::shiftable< bit_set<Block, 1>, std::size_t
> > >
{
public:
        typedef std::size_t key_type;
        typedef std::size_t value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef bit_reference<Block> reference;
        typedef bit_const_reference<Block> const_reference;
        typedef bit_iterator<Block> iterator;
        typedef bit_const_iterator<Block> const_iterator;

        bit_set()
        :
                data_()
        {}

        explicit bit_set(Block const& b)
        :
                data_(b)
        {}

        template<typename InputIt>
        bit_set(InputIt first, InputIt last)
        :
                data_(0)
        {
                BOOST_ASSERT(empty());
                for (auto it = first; it != last; ++it)
                         insert(*it);
        }

        bit_set(std::initializer_list<value_type> init)
        :
                data_(0)
        {
                BOOST_ASSERT(empty());
                for (auto i: init)
                        insert(i);
        }

        iterator begin()
        {
                return iterator(&data_, data_);
        }

        const_iterator begin() const
        {
                return const_iterator(&data_, data_);
        }

        const_iterator cbegin() const
        {
                return const_iterator(&data_, data_);
        }

        iterator end()
        {
                return iterator(&data_, Block(0));
        }

        const_iterator end() const
        {
                return const_iterator(&data_, Block(0));
        }

        const_iterator cend() const
        {
                return const_iterator(&data_, Block(0));
        }

        bool empty() const
        {
                return data_ == 0;
        }

        size_type size() const
        {
                return __builtin_popcountll(data_);
        }

        size_type max_size() const
        {
                return 8 * sizeof(Block);
        }

        void clear()
        {
                data_ = 0;
                BOOST_ASSERT(empty());
        }

        std::pair<iterator, bool> insert(value_type const& value)
        {
                data_ |= element(value);
                return std::make_pair(iterator(&data_, data_), true);
        }

        std::pair<iterator, bool> insert(value_type&& value)
        {
                data_ |= element(std::move(value));
                return std::make_pair(iterator(&data_, data_), true);
        }

        iterator insert(const_iterator /*hint*/, value_type value)
        {
                insert(value);
                return iterator(&data_, data_);
        }

        template<typename InputIt>
        void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        insert(*it);
        }

        void insert(std::initializer_list<value_type> const& ilist)
        {
                for (auto const& i: ilist)
                        insert(i);
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
        }

        void swap(bit_set& other)
        {
                using std::swap;
                swap(data_, other.data_);
        }

        bool count(key_type const& key) const
        {
                return (data_ & element(key)) != 0;
        }

        iterator find(key_type const& key)
        {
                auto result = data_ & element(key);
                return result? iterator(&data_, result) : end();
        }

        const_iterator find(key_type const& key) const
        {
                auto result = data_ & element(key);
                return result? const_iterator(&data_, result) : end();
        }

        // operator!= automatically generated by boost::totally_ordered
        friend bool operator==(bit_set const& lhs, bit_set const& rhs)
        {
                return lhs.data_ == rhs.data_;
        }

        // operator>=, operator>, operator<= automatically generated by boost::totally_ordered
        friend bool operator<(bit_set const& lhs, bit_set const& rhs)
        {
                return lhs.data_ < rhs.data_;
        }

        // operator& automatically generated by boost::bitwise
        bit_set& operator&=(bit_set const& other)
        {
                data_ &= other.data_;
                return *this;
        }

        // operator| automatically generated by boost::bitwise
        bit_set& operator|=(bit_set const& other)
        {
                data_ |= other.data_;
                return *this;
        }

        // operator^ automatically generated by boost::bitwise
        bit_set& operator^=(bit_set const& other)
        {
                data_ ^= other.data_;
                return *this;
        }

        // operator<< automatically generated by boost::shiftable
        bit_set& operator<<=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ <<= n;
                return *this;
        }

        // operator>> automatically generated by boost::shiftable
        bit_set& operator>>=(std::size_t n)
        {
                BOOST_ASSERT(n < max_size());
                data_ >>= n;
                return *this;
        }

        bit_set& flip()
        {
                data_ = ~data_;
                return *this;
        }

private:
        bool invariant() const
        {
                return std::is_sorted(begin(), end()) && std::distance(begin(), end()) == size();
        }

        Block element(key_type const& key) const
        {
                BOOST_ASSERT(key < max_size());
                return Block(1) << key;
        }

        Block data_;
};

template<typename Block, std::size_t N>
void swap(bit_set<Block, N>& lhs, bit_set<Block, N>& rhs)
{
        lhs.swap(rhs);
}

template<typename Block, std::size_t N>
bit_set<Block, N> operator~(bit_set<Block, N> const& lhs)
{
        return bit_set<Block, N>(lhs).flip();
}

template<typename Block, std::size_t N>
bit_set<Block, N> set_intersection(bit_set<Block, N> const& lhs, bit_set<Block, N> const& rhs)
{
        return lhs & rhs;
}

template<typename Block, std::size_t N>
bit_set<Block, N> set_union(bit_set<Block, N> const& lhs, bit_set<Block, N> const& rhs)
{
        return lhs | rhs;
}

template<typename Block, std::size_t N>
bit_set<Block, N> set_symmetric_difference(bit_set<Block, N> const& lhs, bit_set<Block, N> const& rhs)
{
        return lhs ^ rhs;
}

template<typename Block, std::size_t N>
bit_set<Block, N> set_difference(bit_set<Block, N> const& lhs, bit_set<Block, N> const& rhs)
{
        return lhs & ~rhs;
}

template<typename Block, std::size_t N>
bool includes(bit_set<Block, N> const& lhs, bit_set<Block, N> const& rhs)
{
        return set_difference(rhs, lhs).empty();
}

}       // namespace dctl
