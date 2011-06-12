#pragma once
#include <array>
#include <cstddef>

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

        // constructors
        VectorArray(void);
        explicit VectorArray(size_type);

        // iterators
        iterator begin(void);
        const_iterator begin(void) const;
        iterator end(void);
        const_iterator end(void) const;
        reverse_iterator rbegin(void);
        const_reverse_iterator rbegin(void) const;
        reverse_iterator rend(void);
        const_reverse_iterator rend(void) const;

        // capacity
        size_type size(void) const;
        size_type max_size(void) const;
        void resize(size_type);
        size_type capacity(void) const;
        bool empty(void) const;
        bool full(void) const;

        // element access
              reference operator[](size_type);
        const_reference operator[](size_type) const;
              reference front(void);
        const_reference front(void) const;
              reference back(void);
        const_reference back(void) const;

        // modifiers
        void push_back(const_reference);
        void pop_back(void);
        void increment_size(void);
        void decrement_size(void);
        void clear(void);

private:
        // implementation
        bool invariant(void) const;
        bool within_range(size_type) const;
        bool within_bounds(size_type) const;

        // representation
        std::array<T, N> array_;
        size_type size_;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "VectorArray.hpp"
