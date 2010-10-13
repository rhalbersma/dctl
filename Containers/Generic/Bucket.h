#pragma once
#include "../../Utilities/IntegerTypes.h"
#include <iterator>

template<typename T, size_t N>
class Bucket
{
public:
        // typedefs
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // constructors
        Bucket(void) {};

        // iterators
        iterator begin(void);
        const_iterator begin(void) const;
        iterator end(void);
        const_iterator end(void) const;
        reverse_iterator rbegin(void);
        const_reverse_iterator rbegin(void) const;
        reverse_iterator rend(void);
        const_reverse_iterator rend(void) const;

        // capacity: size(), max_size(), resize(), empty() and reserve() not supported
        size_t capacity(void) const;

        // element access: at() not supported
              T& operator[](size_t);
        const T& operator[](size_t) const;
              T& front(void);
        const T& front(void) const;
              T& back(void);
        const T& back(void) const;

        // modifiers: not supported

private:
        bool in_range(size_t) const;

        // representation
        T d_bucket[N];
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Bucket.hpp"
