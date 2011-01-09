#pragma once
#include "../Containers/VectorBucket.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/Ply.h"

class Repetition
{
public:
        // constructors
        Repetition(void) {};

        // predicates
        bool non_unique_back(size_t) const;

        // element access
        HashIndex& hash_index(void);
        HashIndex hash_index(void) const;

        // modifiers
        void copy_back(void);
        void push_back(HashIndex);
        void pop_back(void);

private:
        // implementation
        typedef VectorBucket<HashIndex, PLY_MAX>::const_iterator const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        const_reverse_iterator rbegin(void) const;
        const_reverse_iterator rend(void) const;

        const static size_t MIN_CYCLE = 4;
        const static size_t STRIDE = 2;

        // representation
        VectorBucket<HashIndex, PLY_MAX> repetition_;
};
