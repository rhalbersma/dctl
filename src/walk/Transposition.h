#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace walk {

class Transposition
{
public:
        // constructors
        Transposition();
        Transposition(NodeCount, int);

        // views
        NodeCount leafs() const;                // leaf node count
        int depth() const;                      // remaining depth to search

private:
        // implementation
        static const int LEAFS_BITS = 59;       // up to 5.764608e+17 leaf nodes without overflow
        static const int DEPTH_BITS =  5;       // up to 32 ply depth

        static const int LEAFS_SHIFT = 0;
        static const int DEPTH_SHIFT = LEAFS_SHIFT + LEAFS_BITS;

        static const NodeCount LEAFS_MASK = ((NodeCount(1) << LEAFS_BITS) - 1);
        static const NodeCount DEPTH_MASK = ((NodeCount(1) << DEPTH_BITS) - 1);

        // representation
        NodeCount entry_;                       // leaf node count and depth
};

}       // namespace walk
}       // namespace dctl
