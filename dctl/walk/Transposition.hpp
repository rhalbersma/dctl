#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace walk {

struct Transposition
{
public:
        // structors
        Transposition()
        :
                entry_(0)
        {
        }

        Transposition(NodeCount n, int d)
        :
                entry_(0)
        {
                entry_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
                entry_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
        }

        // leaf node count
        NodeCount leafs() const
        {
                return ((entry_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT);
        }

        // remaining depth to search
        int depth() const
        {
                return (static_cast<int>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT));
        }

private:
        // implementation
        BOOST_STATIC_CONSTANT(auto, LEAFS_BITS = 59);   // up to 5.764608e+17 leaf nodes without overflow
        BOOST_STATIC_CONSTANT(auto, DEPTH_BITS =  5);   // up to 32 ply depth

        BOOST_STATIC_CONSTANT(auto, LEAFS_SHIFT = 0);
        BOOST_STATIC_CONSTANT(auto, DEPTH_SHIFT = LEAFS_SHIFT + LEAFS_BITS);

        BOOST_STATIC_CONSTANT(auto, LEAFS_MASK = ((NodeCount(1) << LEAFS_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, DEPTH_MASK = ((NodeCount(1) << DEPTH_BITS) - 1));

        // representation
        NodeCount entry_;                               // leaf node count and depth
};

}       // namespace walk
}       // namespace dctl
