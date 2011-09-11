#pragma once
#include <boost/config.hpp>
#include "../utility/IntegerTypes.h"

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
        BOOST_STATIC_CONSTANT(auto, LEAFS_BITS = 59);   // up to 5.764608e+17 leaf nodes without overflow
        BOOST_STATIC_CONSTANT(auto, DEPTH_BITS =  5);   // up to 32 ply depth

        BOOST_STATIC_CONSTANT(auto, LEAFS_SHIFT = 0);
        BOOST_STATIC_CONSTANT(auto, DEPTH_SHIFT = LEAFS_SHIFT + LEAFS_BITS);

        BOOST_STATIC_CONSTANT(auto, LEAFS_MASK = ((NodeCount(1) << LEAFS_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, DEPTH_MASK = ((NodeCount(1) << DEPTH_BITS) - 1));

        // representation
        NodeCount entry_;                       // leaf node count and depth
};

}       // namespace walk
}       // namespace dctl
