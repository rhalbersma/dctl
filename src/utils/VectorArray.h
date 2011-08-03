#pragma once
#include <array>
#include <cstddef>

namespace dctl {

template<typename T, std::size_t N>
class VectorArray
{
public:
        // typedefs
        typedef typename std::array<T, N>::iterator iterator;
        typedef typename std::array<T, N>::const_iterator const_iterator;
        typedef typename std::array<T, N>::reverse_iterator reverse_iterator;
        typedef typename std::array<T, N>::const_reverse_iterator const_reverse_iterator;
        typedef typename std::array<T, N>::reference reference;
        typedef typename std::array<T, N>::const_reference const_reference;
        typedef typename std::array<T, N>::size_type size_type;
        typedef typename std::array<T, N>::value_type value_type;

        // constructors
        VectorArray();
        explicit VectorArray(size_type);

        // iterators
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        // capacity
        size_type size() const;
        size_type max_size() const;
        void resize(size_type);
        size_type capacity() const;
        bool empty() const;
        bool full() const;

        // element access
              reference operator[](size_type);
        const_reference operator[](size_type) const;
              reference front();
        const_reference front() const;
              reference back();
        const_reference back() const;

        // modifiers
        void push_back(const_reference);
        void pop_back();
        void increment_size();
        void decrement_size();
        void clear();

private:
        // implementation
        bool invariant() const;
        bool within_range(size_type) const;
        bool within_bounds(size_type) const;

        // representation
        std::array<T, N> array_;
        size_type size_;
};

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "VectorArray.hpp"
