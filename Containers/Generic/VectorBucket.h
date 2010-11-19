#pragma once
#include "Bucket.h"
#include "../../Utilities/IntegerTypes.h"
#include <iterator>

template<typename T, size_t N>
class VectorBucket
{
public:
        // typedefs
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

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
              T& operator[](size_t);
        const T& operator[](size_t) const;
              T& front(void);
        const T& front(void) const;
              T& back(void);
        const T& back(void) const;

        // modifiers: assign(), insert(), erase() and swap() not supported
        void copy_back(void);
        void push_back(const T&);
        void push(void);
        void pop_back(void);
        void clear(void);

private:
        bool invariant(void) const;
        bool in_range(size_t) const;

        // representation
        Bucket<T, N> d_bucket;
        size_t d_size;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "VectorBucket.hpp"
