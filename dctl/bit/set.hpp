#pragma once
#include <cstddef>                      // size_t
#include <cstdint>

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

template<typename T>
class bit_const_iterator
:
        public std::iterator
        <
                std::forward_iterator_tag, // TODO: bidirectional_iterator_tag
                std::size_t,
                std::ptrdiff_t,
                bit_const_iterator<T>,
                bit_const_reference<T>
        >
{
public:
        bit_const_iterator(T const* p, T m)
        :
                pseg_(p),
                mask_(m)
        {}

        bit_const_reference<T> operator*() const
        {
                return bit_const_reference<T>(*pseg_, mask_);
        }

        bit_const_iterator& operator++()
        {
                mask_ &= mask_ - 1;
                return *this;
        }

        // TODO operator-- and the postfix increment/decrement versions

        friend bool operator==(bit_const_iterator const& lhs, bit_const_iterator const& rhs)
        {
                return lhs.pseg_ == rhs.pseg_ && lhs.mask_ == rhs.mask_;
        }

        friend bool operator!=(bit_const_iterator const& lhs, bit_const_iterator const& rhs)
        {
                return !(lhs == rhs);
        }

private:
        T const* pseg_;
        T mask_;
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
                std::forward_iterator_tag, // TODO: bidirectional_iterator_tag
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
                // guard against invalidation
                mask_ &= *pseg_ & (mask_ - 1);
                return *this;
        }

        // TODO operator-- and the postfix increment/decrement versions

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

template<typename T, std::size_t N>
class bit_set;

template<typename T>
class bit_set<T, 1>
{
public:
        typedef bit_iterator<T> iterator;
        typedef bit_const_iterator<T> const_iterator;
        typedef uint64_t value_type;

        bit_set()
        :
                data_()
        {}

        explicit bit_set(T const& b)
        :
                data_(b)
        {}

        explicit bit_set(std::initializer_list<std::size_t> const& il)
        :
                data_(0)
        {
                for (auto i: il)
                        data_ ^= T(1) << i;
        }

        iterator begin()
        {
                return iterator(&data_, data_);
        }

        iterator end()
        {
                return iterator(&data_, T(0));
        }

        const_iterator begin() const
        {
                return const_iterator(&data_, data_);
        }

        const_iterator end() const
        {
                return const_iterator(&data_, T(0));
        }

        bool empty() const
        {
                return data_ == 0;
        }

        std::size_t size() const
        {
                return __builtin_popcountll(data_);
        }

        std::size_t max_size() const
        {
                return 8 * sizeof(T);
        }

        const_iterator find(std::size_t n) const // TODO: make const (requires const arguments for constructor)
        {
                auto result = data_ & (T(1) << n);
                return result? const_iterator(&data_, result) : end();
        }

        bool count(std::size_t n) const
        {
                return (data_ & (T(1) << n)) != 0;
        }

        std::pair<iterator, bool> insert(std::size_t n)
        {
                data_ |= T(1) << n;
                return std::make_pair(iterator(&data_, data_), true);
        }

        iterator insert(const_iterator /*hint*/, std::size_t n)
        {
                insert(n);
                return iterator(&data_, data_);
        }

        template<typename InputIt>
        void insert(InputIt first, InputIt last)
        {
                for (auto it = first; it != last; ++it)
                        insert(*it);
        }

        void insert(std::initializer_list<std::size_t> const& il)
        {
                for (auto i: il)
                        data_ |= T(1) << i;
        }

        void clear()
        {
                data_ = 0;
        }

        void erase(std::size_t n)
        {
                data_ &= ~(T(1) << n);
        }

        bit_set& operator&=(bit_set const& other)
        {
                data_ &= other.data_;
                return *this;
        }

        bit_set& operator|=(bit_set const& other)
        {
                data_ |= other.data_;
                return *this;
        }

        bit_set& operator^=(bit_set const& other)
        {
                data_ ^= other.data_;
                return *this;
        }

        bit_set& flip()
        {
                data_ = ~data_;
                return *this;
        }

private:
        T data_;
};

typedef bit_set<uint64_t, 1> bitset;

bitset operator&(bitset const& lhs, bitset const& rhs)
{
        return bitset(lhs) &= rhs;
}

bitset operator|(bitset const& lhs, bitset const& rhs)
{
        return bitset(lhs) |= rhs;
}

bitset operator^(bitset const& lhs, bitset const& rhs)
{
        return bitset(lhs) ^= rhs;
}

bitset operator~(bitset const& lhs)
{
        return bitset(lhs).flip();
}

bitset set_difference(bitset const& lhs, bitset const& rhs)
{
        return lhs & ~rhs;
}

bool includes(bitset const& lhs, bitset const& rhs)
{
        return set_difference(rhs, lhs).empty();
}

bitset set_intersection(bitset const& lhs, bitset const& rhs)
{
        return lhs & rhs;
}

bitset set_union(bitset const& lhs, bitset const& rhs)
{
        return lhs | rhs;
}

bitset set_symmetric_difference(bitset const& lhs, bitset const& rhs)
{
        return lhs ^ rhs;
}

}       // namespace dctl
