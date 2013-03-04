#pragma once
#include <cstddef>                      // size_t

namespace dctl {
namespace bit {

template<typename> class reference;
template<typename> class iterator;

template<typename T>
class reference
{
public:
        reference(T* p, std::size_t n)
        :
                pseg_(p),
                ctz_(n)
        {}

        operator std::size_t()
        {
                return ctz_;
        }

        iterator<T> operator&()
        {
                return iterator<T>(pseg_, ctz_);
        }

private:
        T* pseg_;
        std::size_t ctz_;
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

        reference<T> operator*()
        {
                return reference<T>(pseg_, ctz_);
        }

        iterator& operator++()
        {
                auto mask = *pseg_ & ((~T(0) ^ T(1)) << ctz_);
                ctz_ = mask? __builtin_ctzll(mask) : 8 * sizeof(T);
                return *this;
        }

        friend bool operator==(iterator lhs, iterator rhs)
        {
                return lhs.pseg_ == rhs.pseg_ && lhs.ctz_ == rhs.ctz_;
        }

        friend bool operator!=(iterator lhs, iterator rhs)
        {
                return !(lhs == rhs);
        }

private:
        T* pseg_;
        std::size_t ctz_;
};

template<typename T, std::size_t N>
class set;

template<typename T>
class set<T, 1>
{
public:
        // structors

        explicit set(T const& b)
        :
                data_(b)
        {}

        explicit set(std::initializer_list<std::size_t> const& il)
        :
                data_(0)
        {
               for (auto x: il)
                   data_ ^= T(1) << x;
        }

        iterator<T> begin()
        {
                return iterator<T>(&data_, __builtin_ctzll(data_));
        }

        iterator<T> end()
        {
                return iterator<T>(&data_, 8 * sizeof(T));
        }

private:
        T data_;
};

}       // namespace bit
}       // namespace dctl
