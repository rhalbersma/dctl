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

        operator bool() const
        {
                return static_cast<bool>(*pseg_ & mask_);
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
        iterator(T* p, unsigned n)
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
        unsigned ctz_;
};

template<typename T, std::size_t N>
class container;

template<typename T>
class container<T, 1>
{
public:
        // structors

        explicit container(T const& b)
        :
                data_(b)
        {}

        // modifiers

        container& operator<<=(std::size_t n)
        {
                data_ <<= n;
                return *this;
        }

        container& operator>>=(std::size_t n)
        {
                data_ >>= n;
                return *this;
        }

        container& operator&=(container const& rhs)
        {
                data &= rhs;
                return *this;
        }

        container& operator|=(container const& rhs)
        {
                data |= rhs;
                return *this;
        }

        container& operator^=(container const& rhs)
        {
                data ^= rhs;
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

        std::size_t size() const
        {
                return count(data_);
        }

private:
        T data_;
};

}       // namespace bit
}       // namespace dctl
