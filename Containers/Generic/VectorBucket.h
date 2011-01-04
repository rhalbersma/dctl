#pragma once
#include "../../Utilities/IntegerTypes.h"
#include <array>

template<typename T, size_t N>
class VectorBucket
{
public:
        // typedefs
        typedef typename std::tr1::array<T, N>::iterator iterator;
        typedef typename std::tr1::array<T, N>::const_iterator const_iterator;
        typedef typename std::tr1::array<T, N>::reverse_iterator reverse_iterator;
        typedef typename std::tr1::array<T, N>::const_reverse_iterator const_reverse_iterator;
        typedef typename std::tr1::array<T, N>::reference reference;
        typedef typename std::tr1::array<T, N>::const_reference const_reference;

        // constructors
        VectorBucket(void);
        explicit VectorBucket(size_t);

        // iterators
        iterator begin(void);
        const_iterator begin(void) const;
        iterator end(void);
        const_iterator end(void) const;
        reverse_iterator rbegin(void);
        const_reverse_iterator rbegin(void) const;
        reverse_iterator rend(void);
        const_reverse_iterator rend(void) const;

        // capacity: max_size() and reserve() not supported
        size_t size(void) const;
        void resize(size_t);
        size_t capacity(void) const;
        bool empty(void) const;
        bool full(void) const;

        // element access: at() not supported
              reference operator[](size_t);
        const_reference operator[](size_t) const;
              reference front(void);
        const_reference front(void) const;
              reference back(void);
        const_reference back(void) const;

        // modifiers: assign(), insert(), erase() and swap() not supported
        void copy_back(void);
        void push_back(const_reference);
        void push(void);
        void pop_back(void);
        void clear(void);

private:
        bool invariant(void) const;
        bool within_range(size_t) const;
        bool within_bounds(size_t) const;

        // representation
        std::tr1::array<T, N> bucket_;
        size_t size_;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "VectorBucket.hpp"
