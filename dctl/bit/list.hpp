#pragma once

namespace dctl {
namespace bit {

template<typename T>
class reference
{
public:
        reference(T* p, T m)
        :
                pseg_(p),
                mask_(m)
        {}

        operator std::size_t() const
        {
                return ctz(mask_);
        }

        iterator<T> operator&() const
        {
                return iterator<T>(pseg_, mask_);
        }

private:
        T* pseg_;
        T mask_;
};

template<typename T>
class iterator
{
public:
        iterator(T* p, std::size_t n)
        :
                pseg_(p),
                ctz_(n)
        {}

        reference<T> operator*() const
        {
                return reference<T>(pseg_, singlet<T>(ctz_));
        }

        iterator operator++()
        {
                return iterator(pseg_, ctz(*pseg_ & ((~T(0) ^ T(1)) << ctz_)));
        }

private:
        T* pseg_;
        std::size_t ctz_;
};

template<typename T, std::size_t N>
class list;

template<typename T>
class list<T, 1>
{
public:
        // structors

        explicit list(T const& b)
        :
                data_(b)
        {}

        // modifiers

        list& operator<<=(std::size_t n)
        {
                data_ <<= n;
                return *this;
        }

        list& operator>>=(std::size_t n)
        {
                data_ >>= n;
                return *this;
        }

        list& operator&=(list const& rhs)
        {
                data_ &= rhs;
                return *this;
        }

        list& operator^=(list const& rhs)
        {
                data_ ^= rhs;
                return *this;
        }

        list& operator|=(list const& rhs)
        {
                data_ |= rhs;
                return *this;
        }

        iterator<T> begin() const
        {
                return iterator<T>(&data_, ctz(data_));
        }

        iterator<T> end() const
        {
                return iterator<T>(&data_, num_bits<T>::value - clz(data_));
        }

        reference<T> front() const
        {
                return *begin();
        }

        reference<T> back() const
        {
                return *end();
        }

        std::size_t empty() const
        {
                return data_ != 0;
        }

        std::size_t size() const
        {
                return count(data_);
        }

private:
        T data_;
};

}       // namespace bit
}       // namespace dctl
