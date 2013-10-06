#pragma once
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

struct Transposition
{
public:
        // structors
        Transposition() = default;

        Transposition(NodeCount n, int d)
        {
                entry_ ^= (n & NODES_MASK) << NODES_SHIFT;
                entry_ ^= (static_cast<NodeCount>(d) & DEPTH_MASK) << DEPTH_SHIFT;
        }

        // leaf node count
        NodeCount nodes() const
        {
                return (entry_ & (NODES_MASK << NODES_SHIFT)) >> NODES_SHIFT;
        }

        // remaining depth to search
        int depth() const
        {
                return static_cast<int>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
        }

private:
        // implementation
        static const auto NODES_BITS = 59;      // up to 5.764608e+17 leaf nodes without overflow
        static const auto DEPTH_BITS =  5;      // up to 32 ply depth

        static const auto NODES_SHIFT = 0;
        static const auto DEPTH_SHIFT = NODES_SHIFT + NODES_BITS;

        static const auto NODES_MASK = ((NodeCount{1} << NODES_BITS) - 1);
        static const auto DEPTH_MASK = ((NodeCount{1} << DEPTH_BITS) - 1);

        // representation
        NodeCount entry_ = 0;                   // leaf node count and depth
};

}       // namespace walk
}       // namespace dctl
