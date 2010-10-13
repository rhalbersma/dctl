#pragma once
#include "../Utilities/IntegerTypes.h"

class PerftNode
{
public:
        // constructors
        PerftNode(void);
        PerftNode(uint64_t, size_t);

        // views
        uint64_t leafs(void) const;             // perft count
        size_t depth(void) const;               // remaining depth to search

        // predicate
        bool is_depth_equal_to(size_t) const;   // check for sufficient depth

private:
        // implementation
        static const size_t LEAFS_BITS = 59;
        static const size_t DEPTH_BITS =  5;

        static const size_t LEAFS_SHIFT = 0;
        static const size_t DEPTH_SHIFT = LEAFS_SHIFT + LEAFS_BITS;

        static const uint64_t LEAFS_MASK = ((uint64_t(1) << LEAFS_BITS) - 1);
        static const uint64_t DEPTH_MASK = ((uint64_t(1) << DEPTH_BITS) - 1);

        // representation
        uint64_t d_perft_node;                  // depth and count
};
