#pragma once
#include "../../Utilities/IntegerTypes.h"

namespace tree {
namespace walk {

class Entry
{
public:
        // constructors
        Entry(void);
        Entry(NodeCount, int);

        // views
        NodeCount leafs(void) const;            // leaf node count
        int depth(void) const;                  // remaining depth to search

private:
        // implementation
        static const size_t LEAFS_BITS = 59;    // up to 5.764608e+17 leaf nodes without overflow
        static const size_t DEPTH_BITS =  5;    // up to 32 ply depth

        static const size_t LEAFS_SHIFT = 0;
        static const size_t DEPTH_SHIFT = LEAFS_SHIFT + LEAFS_BITS;

        static const NodeCount LEAFS_MASK = ((NodeCount(1) << LEAFS_BITS) - 1);
        static const NodeCount DEPTH_MASK = ((NodeCount(1) << DEPTH_BITS) - 1);

        // representation
        NodeCount entry_;                        // leaf node count and depth
};

}       // namespace walk
}       // namespace tree
